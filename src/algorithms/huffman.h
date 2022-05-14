#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

int ascii = 256;
map<string, string> codes;

// Falta -> imprimir a árvore e gerar o arquivo e fazer o decode

// Individual Node
struct Node
{
    string data;
    int freq;

    Node *left, *right;

    Node(string data, unsigned int freq)
    {
        this->data = data;
        this->freq = freq;

        left = right = NULL;
    }
};

// less than - comparison of two node (frequencies)
struct lt
{
    bool operator()(Node *n1, Node *n2)

    {
        return ((n1->freq) > (n2->freq));
    }
};

Node* Huffman(string& data, map<char, int>& freq)
{
    int size = sizeof(data);
    struct Node *left, *right, *top, *root;
    

    priority_queue<Node *, vector<Node *>, lt> tree;

    string n;
    int f;

    // one node per letter
    for (pair<char, int> entry : freq)
    {
        string c = string(1, entry.first); // cast to string
        int freq_c = entry.second;
        tree.push(new Node(c, freq_c));
    }

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

    codes.clear();
    init_codes(root,"");
    return root;
}

void init_codes(struct Node* root, string line)
{
    if (root->left == NULL) // leaf
        codes[root->data]=line;
        return;        

    init_codes(root->left, line + "0");
    init_codes(root->right, line + "1");
}

map<char, int> compute_freqs(string& txt)
{
    map<char, int> f_ascii;

    for (char c : txt)
    {
        f_ascii[c] += 1;
    }

    return f_ascii;
}

string encodeLine(string &txt) { // tem que passar isso pra bytes (o output)

    map<char, int> freqs = compute_freqs(txt);
    Node* root = Huffman(txt, freqs);
    string ans;

    for (auto i: txt)
        ans += codes[to_string(i)];

    return ans;
}

void encodeFile(string &file_path) {
    string output_file_path = file_path + ".myzip";
    string line;
    ifstream* txt_file = new ifstream();
    ofstream* output_file = new ofstream();
    txt_file->open(file_path);
    output_file->open(output_file_path);

    while(!txt_file->eof()) {
        getline(*txt_file, line);
        if (line.empty()) {
            continue;
        }
        string cur = encodeLine(line); // tem que codificar árvore pra salvar no arquivo
        *output_file << cur << '\n';
    }

    int original_file_size = std::__fs::filesystem::file_size(file_path);
    int compressed_file_size = std::__fs::filesystem::file_size(output_file_path);
    printf("%s (%u bytes) file compressed as %s (%u bytes)\n",
        file_path.c_str(), original_file_size,
        output_file_path.c_str(), compressed_file_size
    );
}

string decodeLine(string &line){ // receber em bytes e retornar string

// reconstruir árvore

}

void decodeFile(string &file_path) {
    size_t pos = file_path.find(".myzip");
    string output_file_path = file_path.substr(0,pos);
    
    string line, cur;
    ifstream* txt_file = new ifstream();
    ofstream* output_file = new ofstream();

    txt_file->open(file_path);
    output_file->open(output_file_path);

    while(!txt_file->eof()) {
        getline(*txt_file, line);

        cur = decodeLine(line);

        *output_file << cur << '\n';
    }    
    
}
void test()
{
    string txt = "aaa";
    map<char, int> freqs = compute_freqs(txt);
    Huffman(txt, freqs);
}
