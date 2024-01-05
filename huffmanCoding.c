#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *calc_freq(char str[], int freqs[256]){
    // Calculate the frequencies of chars in a string
    for(int i = 0; i < 256; i ++){
        freqs[i] = 0;
    }
    for(int i = 0; i < strlen(str) ; i++){
        freqs[str[i]] = freqs[str[i]] + 1;
    }
    return freqs;
}

int num_of_non_zero_freq(int freqs[256]){
    int num_of_non_zero = 0;
    for(int i = 0; i < 256; i++){
        if(freqs[i] > 0){
            num_of_non_zero ++;
        }
    }
    return num_of_non_zero;
}

char* array_of_signs(int freqs[256], int N){
    int j = 0;
    char* sign_array = (char*)malloc(sizeof(char)*N);
    for(int i = 0; i < 256; i++){
        if(freqs[i] > 0 ){
            sign_array[j] = i;
            j ++;
        }
    }  
    return sign_array;
}

int* array_of_freqs(int freqs[256], int N){
    int j = 0;
    int* freq_array = (int*)malloc(sizeof(int)*N);
    for(int i = 0; i < 256; i++){
        if(freqs[i] > 0 ){
            freq_array[j] = freqs[i];
            j++;
        }
    }  
    return freq_array;
}

typedef struct Huffman_node {
    char data;
    int freq;
    struct Huffman_node *left;
    struct Huffman_node *right;
} Huffman_node;

Huffman_node *create_node(int freq, char data){
    // Dynamicaly allocates and returns the pointer!
    // ALWAYS REMEMBER: 
    // when you have a pointer to a structure in C or C++, you use the -> operator to access and modify the structure's attributes (members)
    Huffman_node* newNode = (Huffman_node*)malloc(sizeof(Huffman_node));
    if(newNode == NULL){
        printf("Memory allocaition failed.\n");
        exit(1);
    }
    newNode->data = data;
    newNode->freq = freq;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void swap(Huffman_node** a, Huffman_node** b){
    // We use double pointers (**) because we're working with an array of pointers,
    // so we need to dereference the array pointer to access individual pointers.
    Huffman_node* temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(Huffman_node **heap, int size, int index) {
    int smallest = index;
    int left_child, right_child;

    while (1) {
        left_child = 2 * index + 1;
        right_child = 2 * index + 2;

        if (left_child < size && heap[left_child]->freq < heap[smallest]->freq)
            smallest = left_child;

        if (right_child < size && heap[right_child]->freq < heap[smallest]->freq)
            smallest = right_child;

        if (smallest != index) {
            swap(&heap[index], &heap[smallest]); // We have pointer to a pointer! Because the heap is array of the pointers! 
                                                 // If heap would be only array, then we would only use heap[index] without & operator...
            index = smallest;
        } else {
            break;
        }
    }
}

void buildMinHeap(Huffman_node *heap[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        minHeapify(heap, size, i);
    }
}

void heapSort(Huffman_node *heap[], int size) {
    buildMinHeap(heap, size);
    // for(int i = 0; i<size; i++)printf("%d,",heap[i]->freq);
    // printf("\n");
    for (int i = size - 1; i >= 0; i--) {
        swap(&heap[0], &heap[i]);
        minHeapify(heap, i, 0);
    }
}

Huffman_node *create_huffman_tree(int *freqs, char *data, int N){
    // Arrange frequencies from lowest to highest.
    // Combine the characters with the lowest frequencies.
    // Repeat until you merge the last two
    int M = N;
    Huffman_node *heap[N]; //We have array of pointers. NOT Huffman_node heap[N], since memory is allocated dynamicaly! Malloc returns the pointer! 
    for(int i = 0; i < N; i++)heap[i] = create_node(freqs[i], data[i]); // Fill the heap with pointers to Huffman_nodes
    while(M > 1){
        heapSort(heap, M); // Sort the heap based on the frequencies
        for(int i = 0; i<M; i++)printf("%d,",heap[i]->freq); // code check
        printf("\n");
        Huffman_node* right = heap[M-1];
        Huffman_node* left = heap[M-2];
        heap[M-2] = create_node(left->freq + right->freq, '\0');
        heap[M-2]->left = left;
        heap[M-2]->right = right;
        M--;
    }
    return heap[0];
}

void generateHuffmanCodes(Huffman_node* root, int arr[], int top, char* huffmanCodes[]) {
    if (root->left) {
        arr[top] = 0;
        generateHuffmanCodes(root->left, arr, top + 1, huffmanCodes);
    }
    if (root->right) {
        arr[top] = 1;
        generateHuffmanCodes(root->right, arr, top + 1, huffmanCodes);
    }
    if (!root->left && !root->right) {
        // Create a string representation of the Huffman code
        char* code = (char*)malloc((top + 1) * sizeof(char));
        if (code == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        for (int i = 0; i < top; i++) {
            code[i] = arr[i] + '0'; // Convert 0 or 1 to '0' or '1'
        }
        code[top] = '\0'; // Null-terminate the string
        huffmanCodes[(int)(root->data)] = code; // Store the code in the array
    }
}

int main(void) {
    // Initialize variables for input processing
    char *str = NULL;
    size_t buffer_size = 0;

    // Prompt user for input
    printf("Enter a string: ");

    // Read input string with dynamic memory allocation
    if (getline(&str, &buffer_size, stdin) == -1) {
        perror("Error reading input");
        return 1;
    }

    // Remove trailing newline character, if present
    size_t characters_read = strlen(str);
    if (str[characters_read - 1] == '\n') {
        str[characters_read - 1] = '\0';
    }

    // Display the entered string
    printf("You entered: %s\n", str);

    // Calculate frequencies of characters in the string
    int freqs[256];
    calc_freq(str, freqs);
    int N = num_of_non_zero_freq(freqs);

    // Display the number of unique characters
    printf("Number of unique characters: %d\n", N);

    // Process and display character frequencies
    char* sign_array = array_of_signs(freqs, N);
    int* freq_array = array_of_freqs(freqs, N);
    printf("Characters: ");
    for (int i = 0; i < N; i++) {
        printf("%c ", sign_array[i]);
    }
    printf("\nFrequencies: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", freq_array[i]);
    }
    printf("\n");

    // Construct Huffman tree
    Huffman_node *tree = create_huffman_tree(freq_array, sign_array, N);

    // Generate and print Huffman codes
    char* huffmanCodes[256] = { NULL };
    int arr[100], top = 0; // Assuming maximum code length of 100
    printf("Huffman Codes:\n");
    generateHuffmanCodes(tree, arr, top, huffmanCodes);
    for (int i = 0; i < 256; i++) {
        if (huffmanCodes[i] != NULL) {
            printf("%c: %s\n", (char)i, huffmanCodes[i]);
        }
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < 256; i++) {
        free(huffmanCodes[i]);
    }
    free(str); // Free the input string memory

    return 0;
}
