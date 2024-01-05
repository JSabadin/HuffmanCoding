#include <stdio.h>
#include <stdlib.h>

// Define a structure for a Huffman node
struct HuffmanNode {
    char data;
    unsigned frequency;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
};

// Define a structure for a Min Heap Node
struct MinHeapNode {
    struct HuffmanNode* huffmanNode;
    struct MinHeapNode* next;
};

// Define a structure for a Min Heap
struct MinHeap {
    struct MinHeapNode* head;
};

// Function to create a new Huffman node
struct HuffmanNode* createHuffmanNode(char data, unsigned frequency) {
    struct HuffmanNode* newNode = (struct HuffmanNode*)malloc(sizeof(struct HuffmanNode));
    newNode->data = data;
    newNode->frequency = frequency;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to create a new Min Heap Node
struct MinHeapNode* createMinHeapNode(struct HuffmanNode* huffmanNode) {
    struct MinHeapNode* newNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    newNode->huffmanNode = huffmanNode;
    newNode->next = NULL;
    return newNode;
}

// Function to create a new Min Heap
struct MinHeap* createMinHeap() {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->head = NULL;
    return minHeap;
}

// Function to swap two Min Heap Nodes
void swapMinHeapNodes(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain the Min Heap property
void minHeapify(struct MinHeap* minHeap) {
    struct MinHeapNode* current = minHeap->head;
    struct MinHeapNode* smallest = current;

    while (current != NULL) {
        if (current->huffmanNode->frequency < smallest->huffmanNode->frequency) {
            smallest = current;
        }
        current = current->next;
    }

    swapMinHeapNodes(&minHeap->head, &smallest);
}

// Function to check if the Min Heap has only one node left
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->head != NULL && minHeap->head->next == NULL);
}

// Function to extract the node with the smallest frequency from the Min Heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap) {
    struct MinHeapNode* temp = minHeap->head;
    minHeap->head = minHeap->head->next;
    return temp;
}

// Function to insert a new node into the Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode) {
    if (minHeap->head == NULL) {
        minHeap->head = minHeapNode;
        return;
    }

    struct MinHeapNode* current = minHeap->head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = minHeapNode;
    minHeapify(minHeap);
}

// Function to build the Huffman tree
struct HuffmanNode* buildHuffmanTree(char data[], unsigned frequency[], int size) {
    struct MinHeap* minHeap = createMinHeap();

    for (int i = 0; i < size; ++i) {
        struct HuffmanNode* huffmanNode = createHuffmanNode(data[i], frequency[i]);
        struct MinHeapNode* minHeapNode = createMinHeapNode(huffmanNode);
        insertMinHeap(minHeap, minHeapNode);
    }

    while (!isSizeOne(minHeap)) {
        struct MinHeapNode* left = extractMin(minHeap);
        struct MinHeapNode* right = extractMin(minHeap);

        struct HuffmanNode* newNode = createHuffmanNode('$', left->huffmanNode->frequency + right->huffmanNode->frequency);
        newNode->left = left->huffmanNode;
        newNode->right = right->huffmanNode;

        struct MinHeapNode* minHeapNode = createMinHeapNode(newNode);
        insertMinHeap(minHeap, minHeapNode);
    }

    return extractMin(minHeap)->huffmanNode;
}

// Function to print the Huffman codes
void printHuffmanCodes(struct HuffmanNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printHuffmanCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printHuffmanCodes(root->right, arr, top + 1);
    }

    if (!root->left && !root->right) {
        printf("Character: %c, Code: ", root->data);
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

int main() {
    char data[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    unsigned frequency[] = { 5, 91, 12, 13, 16, 45 };
    int size = sizeof(data) / sizeof(data[0]);

    struct HuffmanNode* root = buildHuffmanTree(data, frequency, size);

    int arr[100], top = 0;
    printf("Huffman Codes:\n");
    printHuffmanCodes(root, arr, top);

    return 0;
}
