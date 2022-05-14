#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

// pra mim: rever toda a parte de decode, fazer clear

int ascii = 256;

// ---------- Structures
// Individual Node
struct Node
{
    string data;
    string code;
    int freq;

    Node *left, *right;

    Node()
    {
        left = right = NULL;
    }

    Node(string data, unsigned int freq)
    {
        this->data = data;
        this->freq = freq;

        left = right = NULL;
    }
};

priority_queue<Node *, vector<Node *>, lt> baseHeap;
Node *root;

Node *Huffman()
{
    struct Node *left, *right, *top, *root;

    priority_queue<Node *, vector<Node *>, lt> tree = baseHeap;

    string n;

    int f;

    while (tree.size() > 1)
    {
        left = tree.top(); // nera so fazer left = tree.pop ?
        tree.pop();
        right = tree.top();
        tree.pop();

        n = left->data + right->data;
        f = left->freq + right->freq;

        top = new Node(n, f);
        top->left = left;
        top->right = right;

        tree.push(top);
    }

    root = tree.top();
    tree.pop();

    init_codes(root, "");
    return root;
}

// ---------- Utils
// less than - comparison of two node (frequencies)
struct lt
{
    bool operator()(Node *n1, Node *n2)

    {
        return ((n1->freq) > (n2->freq));
    }
};

void init_codes(struct Node *root, string codeline)
{
    if (root->left == NULL) // leaf
    {
        root->code = codeline;
        return;
    }

    init_codes(root->left, codeline + "0");
    init_codes(root->right, codeline + "1");
}

vector<Node *> baseFreq()
{
    vector<Node *> base;
    for (int i = 0; i < ascii; i++)
    {
        base.push_back(new Node(to_string(i), 0));
    }

    return base;
}

void createBaseHeap(string &file_path, vector<Node *> freqs)
{
    char ch;

    ifstream *txt_file = new ifstream();
    txt_file->open(file_path);
    txt_file->get(ch);

    while (!txt_file->eof())
    {
        freqs[ch]->freq++;
        txt_file->get(ch);
    }

    for (int i = 0; i < ascii; i++)
    { // Add only nodes which appear
        if (freqs[i]->freq > 0)
        {
            baseHeap.push(freqs[i]);
        }
    }
}
int binToDec(string str)
{
    int r = 0;
    for (auto c : str)
    {
        r = r * 2 + c - '0';
    }
    return r;
}

string decToBin(int num)
{
    string temp, r = "";
    while (num > 0)
    {
        temp += (num % 2 + '0');
        num /= 2;
    }
    r.append(8 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--)
    {
        r += temp[i];
    }
    return r;
}

void rebuildTree(Node *root, unsigned char *c, string txt)
{ // se der ruim é aqui pq n terminei de ajeitar
    Node *curr = root;

    for (int i = 0; i < txt.length(); i++)
    {
        if (txt[i] == '0')
        {
            if (curr->left == NULL)
            {
                curr->left = new Node();
            }
            curr = curr->left;
        }
        else if (txt[i] == '1')
        {
            if (curr->right == NULL)
            {
                curr->right = new Node();
            }
            curr = curr->right;
        }
    }
    string dt(reinterpret_cast<char *>(c));
    curr->data = dt;
}

void prepareTree(string &file_path)
{
    root = new Node();
    char ch, size;

    ifstream *txt_file = new ifstream();
    txt_file->open(file_path);
    txt_file->get(ch);

    txt_file->read(reinterpret_cast<char *>(&size), 1);

    // next size * (1 + 16) characters contain (char)data and (string)code[in decimal]
    for (int i = 0; i < size; i++)
    {
        char cod;
        unsigned char codec[16];
        txt_file->read(&cod, 1);
        txt_file->read(reinterpret_cast<char *>(codec), 16);

        // converting decimal characters into their binary equivalent to obtain code
        string codeStr = "";
        for (int i = 0; i < 16; i++)
        {
            codeStr += decToBin(codec[i]);
        }
        // Removing padding by ignoring first (127 - curr->code.length()) '0's and next '1' character
        int j = 0;
        while (codec[j] == '0')
        {
            j++;
        }
        codeStr = codeStr.substr(j + 1);
        // Adding node with aCode data and hCodeStr string to the huffman tree
        rebuildTree(root, codec, codeStr);
    }
}

// ---- Functions
void encodeFile(string &file_path)
{

    ifstream *txt_file = new ifstream();
    txt_file->open(file_path);

    string output_file_path = file_path + ".myzip";
    ofstream *output_file = new ofstream();
    output_file->open(output_file_path);

    vector<Node *> freqs = baseFreq();
    createBaseHeap(file_path, freqs);

    Node *root = Huffman();

    string line;
    string relevant, aux = "";
    char ch;

    // saves the meta data
    relevant += (char)baseHeap.size();
    priority_queue<Node *, vector<Node *>, lt> saveHeap(baseHeap);

    while (!saveHeap.empty())
    {
        Node *r = saveHeap.top();
        relevant += r->data;

        // Saving 16 decimal values representing code of current node data
        aux.assign(255 - (r->code.length()), '0'); // MT em duvida se aqui fica 127 mesmo ou 255
        aux += '1';
        aux += r->code;

        // Saving decimal values of every 8-bit binary code
        relevant += (char)binToDec(aux.substr(0, 8));
        for (int i = 0; i < 15; i++)
        {
            aux = aux.substr(8);
            relevant += (char)binToDec(aux.substr(0, 8));
        }
        saveHeap.pop();
    }
    aux.clear();

    // saves codes of every char of the txt file
    txt_file->get(ch);
    while (!txt_file->eof())
    {
        aux += freqs[ch]->code;
        // Saving decimal values of every 8-bit binary code
        while (aux.length() > 8)
        {
            relevant += (char)binToDec(aux.substr(0, 8));
            aux = aux.substr(8);
        }
        txt_file->get(ch);
    }

    // appends 0s until we have 8 bits
    int ovf = 8 - aux.length();
    if (aux.length() < 8)
    {
        aux.append(ovf, '0');
    }
    relevant += (char)binToDec(aux);
    // overflow 0's
    relevant += (char)ovf;

    // writes the relevant string to the output file
    *output_file << relevant;

    int original_file_size = std::__fs::filesystem::file_size(file_path);
    int compressed_file_size = std::__fs::filesystem::file_size(output_file_path);
    printf("%s (%u bytes) file compressed as %s (%u bytes)\n",
           file_path.c_str(), original_file_size,
           output_file_path.c_str(), compressed_file_size);
}

void decodeFile(string &file_path)
{
    char count0;
    unsigned char size, textseg;
    vector<unsigned char> text;

    ifstream *txt_file = new ifstream();
    txt_file->open(file_path);

    string output_file_path = file_path + "aaaaaaa";
    ofstream *output_file = new ofstream();
    output_file->open(output_file_path);

    prepareTree(file_path);

    txt_file->read(reinterpret_cast<char *>(&size), 1);
    // Reading count at the end of the file which is number of bits appended to make final value 8-bit
    txt_file->seekg(-1, ios::end);
    txt_file->read(&count0, 1);
    // Ignoring the meta data (huffman tree) (1 + 17 * size) and reading remaining file
    txt_file->seekg(1 + 17 * size, ios::beg);

    txt_file->read(reinterpret_cast<char *>(&textseg), 1);
    while (!txt_file->eof())
    {
        text.push_back(textseg);
        txt_file->read(reinterpret_cast<char *>(&textseg), 1);
    }

    Node *curr = root;
    string path;
    for (int i = 0; i < text.size() - 1; i++)
    {
        // Converting decimal number to its equivalent 8-bit binary code
        path = decToBin(text[i]);
        if (i == text.size() - 2)
        {
            path = path.substr(0, 8 - count0);
        }
        // Traversing huffman tree and appending resultant data to the file
        for (int j = 0; j < path.size(); j++)
        {
            if (path[j] == '0')
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }

            if (curr->left == NULL && curr->right == NULL)
            { // Found leaf
                *output_file << curr->data;
                curr = root;
            }
        }
    }
}
