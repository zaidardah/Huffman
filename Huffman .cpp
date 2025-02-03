#include <iostream>
#include <string>
using namespace std;

// Structure for Huffman Tree Node
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
};

// Min Priority Queue using a binary heap
struct MinHeap {
    Node* heap[100];
    int size = 0;

    // Insert a node into the heap
    void insert(Node* node) {
        heap[size] = node;
        int i = size++;
        // Heapify up
        while (i > 0 && heap[i]->freq < heap[(i - 1) / 2]->freq) {
            swap(heap[i], heap[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    // Remove and return the node with the smallest frequency
    Node* extractMin() {
        if (size == 0) return nullptr;
        Node* minNode = heap[0];
        heap[0] = heap[--size];
        int i = 0;

        // Heapify down
        while (true) {
            int smallest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;

            if (left < size && heap[left]->freq < heap[smallest]->freq)
                smallest = left;
            if (right < size && heap[right]->freq < heap[smallest]->freq)
                smallest = right;
            if (smallest == i) break;

            swap(heap[i], heap[smallest]);
            i = smallest;
        }

        return minNode;
    }
};

// Count frequency of each character in the text
void countFrequencies(const string& text, int freq[]) {
    for (char c : text) {
        freq[(unsigned char)c]++;
    }
}

// Build Huffman Tree using the frequencies
Node* buildHuffmanTree(int freq[]) {
    MinHeap heap;

    // Create leaf nodes for characters with non-zero frequency
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            heap.insert(new Node{ (char)i, freq[i], nullptr, nullptr });
        }
    }

    // Iterate until the heap contains only one node
    while (heap.size > 1) {
        Node* left = heap.extractMin();
        Node* right = heap.extractMin();
        Node* parent = new Node{ '\0', left->freq + right->freq, left, right };
        heap.insert(parent);
    }

    return heap.extractMin();
}

// Generate Huffman codes by traversing the tree
void generateCodes(Node* root, string code, string codes[]) {
    if (!root) return;

    // If it's a leaf node, store the code
    if (root->ch != '\0') {
        codes[(unsigned char)root->ch] = code;
    }

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// Encode the text using Huffman codes
string encode(const string& text, string codes[]) {
    string encoded;
    for (char c : text) {
        encoded += codes[(unsigned char)c];
    }
    return encoded;
}

// Decode the binary string using the Huffman tree
string decode(Node* root, const string& encoded) {
    string decoded;
    Node* current = root;
    for (char bit : encoded) {
        current = (bit == '0') ? current->left : current->right;

        // If it's a leaf node, append the character and reset to root
        if (!current->left && !current->right) {
            decoded += current->ch;
            current = root;
        }
    }
    return decoded;
}

int main() {
    string text;
    cout << "Enter a paragraph: ";
    getline(cin, text);

    int freq[256] = { 0 };
    countFrequencies(text, freq);

    Node* root = buildHuffmanTree(freq);

    string codes[256];
    generateCodes(root, "", codes);

    cout << "\n*** Huffman Code Table ***\n";
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            cout << "-> '" << (char)i << "' (Freq: " << freq[i]
                 << ", Bits: " << codes[i].length() << "): " << codes[i] << "\n";
        }
    }

    string encodedText = encode(text, codes);
    cout << "\nEncoded paragraph in Huffman code: " << encodedText << "\n";
    cout << "Total bits required: " << encodedText.length() << "\n";

    string toEncode;
    cout << "\nEnter a string to encode using Huffman codes: ";
    getline(cin, toEncode);
    cout << "Encoded String: " << encode(toEncode, codes) << "\n";

    string toDecode;
    cout << "\nEnter a binary string to decode: ";
    getline(cin, toDecode);
    cout << "Decoded String: " << decode(root, toDecode) << "\n";

    return 0;
}
