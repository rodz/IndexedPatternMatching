#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <map>

using namespace std;

int ascii = 256;

// Falta -> imprimir a árvore e gerar o arquivo e fazer o decode

// Individual Node
struct Node
{
    string data;
    unsigned freq;

    Node *left, *right;

    Node(string data, unsigned freq)
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

void Huffman(string data, map<char, int> freq)
{
    int size = sizeof(data);
    struct Node *left, *right, *top, *root;

    priority_queue<Node *, vector<Node *>, lt> tree;

    string n;
    int f;

    // one node per letter
    for (auto entry : freq)
    {
        string c = entry.first + ""; // cast to string
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
}

map<char, int> compute_freqs(string txt)
{
    map<char, int> f_ascii;

    for (char c : txt)
    {
        f_ascii[c] += 1;
    }

    return f_ascii;
}

void test()
{
    string txt = "aaa";
    map<char, int> freqs = compute_freqs(txt);
    Huffman(txt, freqs);
};