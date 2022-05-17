#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

int ascii = 255;
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

    Node()
    {
        this->data = "root";
        this->freq = 0;

        left = right = NULL;
    }

    void print_node(int level)
    {
        if (this->left == NULL) {
            cout<<string(level, ' ')<<this->data<<' '<<this->freq<<endl;
            return;
        }
        this->left->print_node(level+1);
        cout<<string(level, ' ')<<this->data<<' '<<this->freq<<endl;
        this->right->print_node(level+1);
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

void init_codes(struct Node* root, string line)
{
    if (root->left == NULL) { // leaf
        codes[root->data]=line;
        return; 
    }

    init_codes(root->left, line + "0");
    init_codes(root->right, line + "1");
}

Node* fromCodesToTree(map<unsigned char, string> char_to_code) {
    Node* root = new Node();

    for(auto x: char_to_code) {
        unsigned char cur = x.first;
        string path = x.second;
        Node* cur_root = root;
        for(char bin: path) {
            if(bin == '0') {
                if (cur_root->left == NULL) {
                    cur_root->left = new Node();
                }
                cur_root = cur_root->left;
            }else {
                if (cur_root->right == NULL) {
                    cur_root->right = new Node();
                }
                cur_root = cur_root->right;
            }
        }
        cur_root->data = string(1, cur);
    }

    return root;
}

Node* Huffman(map<unsigned char, int>& freq)
{
    struct Node *left, *right, *top, *root;
    
    priority_queue<Node *, vector<Node *>, lt> tree;

    // one node per letter
    for (pair<unsigned char, int> entry : freq)
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

        string n = left->data + right->data;
        int f = left->freq + right->freq;

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

map<unsigned char, int> compute_freqs(string& file_path)
{
    ifstream* txt_file = new ifstream();
    txt_file->open(file_path);
    string line;
    map<unsigned char, int> f_ascii;

    while(!txt_file->eof()) {
        getline(*txt_file, line);
        if (line.empty()) {
            continue;
        }
        
        for (unsigned char c : line){
            f_ascii[c] += 1;
        }
        f_ascii['\n'] += 1;
    }
    txt_file->close();
    return f_ascii;
}

map<string, string> clean_codes(map<string, string> &code_map) {
    map<string, string> new_codes;
    int max_size = 0;
    for(auto x: code_map) {
        if (x.first.length() > 1) {
            continue;
        }
        int cur_zeros = 0;
        max_size = max(max_size, (int) x.second.length());
    }
    // Chop zeros
    for(auto x: code_map) {
        if (x.first.length() > 1) {
            continue;
        }
        new_codes[x.first] = x.second + string(max_size - x.second.length(), '0');
    }

    return new_codes;
}

void encodeFile(string &file_path) {
    string output_file_path = file_path + ".myzip";
    string line;

    // Create tree
    map<unsigned char, int> freqs = compute_freqs(file_path);
    Node* root = Huffman(freqs);
    // Start write process;
    ifstream* txt_file = new ifstream();
    ofstream* output_file = new ofstream();
    txt_file->open(file_path);
    output_file->open(output_file_path, ios::out | ios::binary);

    // write 8 bit integer, size of heap
    unsigned char qt = (unsigned char) freqs.size();
    output_file->write(reinterpret_cast<char*>(&qt), 1);

    // Write to zip file codes for each char
    int big_code = 0;
    for(auto code: codes) {
        string cur_char = code.first;
        if(cur_char.length() != 1) {
            continue;
        }
        unsigned char cur_char_code = cur_char[0];
        unsigned char cur_code_sz = (unsigned char) code.second.length();
        output_file->write(reinterpret_cast<char*>(&cur_char_code), 1);
        output_file->write(reinterpret_cast<char*>(&cur_code_sz), 1);
        string upt_code = string(cur_code_sz%8? 8 - (cur_code_sz%8) : 0, '0') + code.second;
        // cout<<cur_char_code<<' '<<int(cur_code_sz)<<' '<<code.second<<endl;
        for(int i = 0; i<upt_code.length(); i+=8) {
            string cur_code = upt_code.substr(i, 8);
            unsigned char cur_code_bt = stoi(cur_code, nullptr, 2);
            output_file->write(reinterpret_cast<char*>(&cur_code_bt), 1);
        }
    }

    // Encode file line by line
    string cur_insert = "";
    string to_insert = "";
    while(!txt_file->eof()) {
        getline(*txt_file, line);

        line += '\n';
        for (unsigned char i: line){
            string cur_i = string(1, i);
            string value = codes[cur_i];
            for (unsigned char c: value) {
                cur_insert += c;
                if(cur_insert.length() == 8) {
                    unsigned char value = bitset<8>(cur_insert).to_ulong();
                    output_file->write(reinterpret_cast<char*>(&value), 1);
                    cur_insert = "";
                }
            }
        }
    }
    unsigned char value = bitset<8>(cur_insert).to_ulong();
    output_file->write(reinterpret_cast<char*>(&value), 1);

    int original_file_size = std::__fs::filesystem::file_size(file_path);
    int compressed_file_size = std::__fs::filesystem::file_size(output_file_path);
    printf("%s (%u bytes) file compressed as %s (%u bytes)\n",
        file_path.c_str(), original_file_size,
        output_file_path.c_str(), compressed_file_size
    );
    txt_file->close();
    output_file->close();
}

void decodeFile(string &file_path) {
    string output_file_path = file_path + ".myzipdecoded";
    string line;

    ifstream* txt_file = new ifstream();
    ofstream* output_file = new ofstream();
    txt_file->open(file_path, ios::out | ios::binary);
    output_file->open(output_file_path);

    // Create tree
    unsigned char tree_size;
    map<unsigned char, string> char_to_code;

    txt_file->read(reinterpret_cast<char*>(&tree_size), 1);
    for (int i = 0; i<tree_size; i++) {
        unsigned char char_key, path_size, path_value;
        txt_file->read(reinterpret_cast<char*>(&char_key), 1);
        txt_file->read(reinterpret_cast<char*>(&path_size), 1);
        string path_str = "";
        for(int i = 0; i<path_size; i+=8) {
            txt_file->read(reinterpret_cast<char*>(&path_value), 1);
            string cur_path_str = bitset<8>(path_value).to_string();
            path_str+=cur_path_str;
        }
        path_str = path_str.substr(path_str.length() - path_size, path_size);
        // cout<<char_key<<' '<<int(path_size)<<' '<<path_str<<endl;
        char_to_code[char_key] = path_str;
    }

    Node* root = fromCodesToTree(char_to_code);
    Node* cur_node = root;
    while (!txt_file->eof()) {
        unsigned char text_bin;
        string write_out = "";
        txt_file->read(reinterpret_cast<char*>(&text_bin), 1);
        string text = bitset<8>(text_bin).to_string();
        for (char c: text) {
            if(c == '0') {
                cur_node = cur_node->left;
            } else {
                cur_node = cur_node->right;
            }

            if (cur_node->left == NULL) {
                write_out += cur_node->data;
                cur_node = root;
            }
        }
        *output_file << write_out;
    }

    int original_file_size = std::__fs::filesystem::file_size(file_path);
    int compressed_file_size = std::__fs::filesystem::file_size(output_file_path);
    printf("%s (%u bytes) file decompressed as %s (%u bytes)\n",
        file_path.c_str(), original_file_size,
        output_file_path.c_str(), compressed_file_size
    );
    txt_file->close();
    output_file->close();
}

int test()
{
    string text_file = "oi.txt";
    unsigned char qt = 7;
    // Start write process;
    ofstream* output_file = new ofstream();
    output_file->open(text_file, ios::out | ios::binary);
    cout<<int(qt)<<endl;
    output_file->write(reinterpret_cast<char*>(&qt), 8);
    output_file->close();

    ifstream* txt_file = new ifstream();
    txt_file->open(text_file, ios::out | ios::binary);
    unsigned char from_qt;
    txt_file->read(reinterpret_cast<char*>(&from_qt), 8);
    cout<<int(from_qt)<<endl;
    return 0;
}
