#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <iomanip>

using namespace std;

class Node{
    public:
        char ch;
        int freq;
        Node* left;
        Node* right;
        Node(char ch, int freq, Node* left = nullptr, Node* right = nullptr)
            : ch(ch),
            freq(freq),
            left(left),
            right(right)
        {

        }
};

// for priority queue (heap) ordering. Lower frequency = higher priority
struct compare {
    bool operator()(Node* l, Node* r)
    {
        return l->freq > r->freq;
    }
};

// generate a map of characters with their frequencies
unordered_map<char, int> gen_freq(string text){
    unordered_map<char, int> freq;
    for(char ch : text)
        freq[ch]++;
    
    return freq;
}

// build a tree representing Huffman codes
Node* build_tree(string text){
    unordered_map<char, int> freq = gen_freq(text);
    
    priority_queue<Node*, vector<Node*>, compare> pq;

    for(auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }  // add every node with character and frequency to the queue

    while(pq.size() != 1){
        // remove 2 lowest-frequency nodes
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));  // merge left and right, add their parent node to the queue
    }

    Node* root = pq.top();
    return root;
}

// generate codes by populating the map huff_codes with characters and their corresponding encodings
void gen_codes(Node* root, string str, unordered_map<char, string>& huff_codes){
    if(root == nullptr)
        return;
    
    if(!root->left && !root->right){  // leaf node
        huff_codes[root->ch] = str;
    }

    gen_codes(root->left, str+"0", huff_codes);
    gen_codes(root->right, str+"1", huff_codes);
}

// print every character, frequency, and code
void print_table(unordered_map<char, int>& freq, unordered_map<char, string>& codes, Node* root){
    cout << left << setw(10) << "Symbol" 
         << setw(15) << "Frequency" 
         << "Code" << endl;

    for (auto pair : freq) {
        cout << setw(10) << pair.first 
             << setw(15) << pair.second 
             << codes[pair.first] << endl;
    }
}

// combine the character codes into a single string encoding
string compress(const string& text, unordered_map<char, string>& codes){
    string compr = "";

    for(char c: text)
        compr += codes[c];

    return compr;
}

// decode a string by walking through the tree
string decompress(const string& compr, Node* root){
    string decompr = "";
    Node* node = root;  // start at root

    for(char bit : compr){
        if(bit == '0')
            node = node->left;
        else
            node = node->right;

        if(node->ch != '\0'){  // leaf
            decompr += node->ch;
            node = root;  // go back to root
        }
    }

    return decompr;
}

// delete the tree and free memory
void delete_tree(Node* root) {
    if (root == nullptr) return;
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

int main(){
    string text = "Hello world";

    unordered_map<char, int> freq = gen_freq(text);
    Node* root = build_tree(text);
    unordered_map<char, string> codes;
    gen_codes(root, "", codes);
    

    print_table(freq, codes, root);

    string compr = compress(text, codes);

    cout << "Compressed string: " << compr << endl;

    cout << "Decompressed string: " << decompress(compr, root) << endl;

    delete_tree(root);
    
}


