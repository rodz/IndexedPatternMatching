#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

// pra mim: quando voltar, procura por parei aqui!!
// pra rods: o que ta dando errado-> não to conseguindo criar um heap com base em outro, não sei se é problema de ponteiro - linha 40 :()

int ascii = 256;
priority_queue<Node *, vector<Node *>, lt> baseHeap;

// ---------- Structures 
// Individual Node
struct Node
{
    string data;
    string code;
    int freq;

    Node *left, *right;

    Node(string data, unsigned int freq)
    {
        this->data = data;
        this->freq = freq;

        left = right = NULL;
    }
};
Node *Huffman()
{
    struct Node *left, *right, *top, *root;

    priority_queue<Node *, vector<Node *>, lt> tree(baseHeap);

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

void recreateTree(string &file_path) {
    char ch, size;

    ifstream *txt_file = new ifstream();
    txt_file->open(file_path);
    txt_file->get(ch);


}

int binToDec(string str) {
    int r = 0;
    for (auto c : str) {
        r = r * 2 + c - '0';
    }
    return r;
}

string decToBin(int num) {
    string temp, r = "";
    while (num > 0) {
        temp += (num % 2 + '0');
        num /= 2;
    }
    r.append(8 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--) {
        r += temp[i];
    }
    return r;
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
    ifstream *txt_file = new ifstream();
    txt_file->open(file_path);

    string output_file_path = file_path + "aaaaaaa";
    ofstream *output_file = new ofstream();
    output_file->open(output_file_path);

    recreateTree(file_path);
    // parei aqui
}

