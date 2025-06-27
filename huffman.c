#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define N 256
#define MAX_TEXT 5000
#define MAX_PRIORITY_QUEUE 1280
#define MAX_ENCODED_TEXT 20000

typedef struct Node Node;
typedef struct PriorityQueue PriorityQueue;

struct PriorityQueue {
    int size;
    Node **arr;
};

struct Node {
    char data;
    int freq;
    Node *left;
    Node *right;
};

void swap(PriorityQueue *pq, int i1, int i2) {
    Node *temp = pq->arr[i1];
    pq->arr[i1] = pq->arr[i2];
    pq->arr[i2] = temp;
}

void siftUp(PriorityQueue *pq, int index) {
    if (index == 0) {
        return;
    }
    int parentIndex = (index - 1) / 2;
    if (pq->arr[parentIndex]->freq <= pq->arr[index]->freq) {
        return;
    }
    swap(pq, index, parentIndex);
    siftUp(pq, parentIndex);
}

void siftDown(PriorityQueue *pq, int index) {
    int lastParentIndex = (pq->size) / 2 - 1;
    if (index > lastParentIndex) {
        return;
    }

    int leftChildIndex = index * 2 + 1;
    int rightChildIndex = index * 2 + 2;

    int smallestIndex = index;
    if (leftChildIndex < pq->size && pq->arr[leftChildIndex]->freq < pq->arr[index]->freq) {
        smallestIndex = leftChildIndex;
    }
    if (rightChildIndex < pq->size && pq->arr[rightChildIndex]->freq < pq->arr[smallestIndex]->freq) {
        smallestIndex = rightChildIndex;
    }
    if (smallestIndex == index) {
        return;
    }
    swap(pq, index, smallestIndex);
    siftDown(pq, smallestIndex);
}

Node *poll(PriorityQueue *pq) {
    if (pq->size == 0) {
        return NULL;
    }
    Node *temp = pq->arr[0];

    int i = --(pq->size);
    pq->arr[0] = pq->arr[i];
    pq->arr[i] = NULL;

    siftDown(pq, 0);

    return temp;
}

Node *peek(PriorityQueue *pq) {
    return pq->size == 0 ? NULL : pq->arr[0];
}

void add(PriorityQueue *pq, Node *n) {
    if (n == NULL) {
        return;
    }
    int i = pq->size;
    if (i + 1 >= MAX_PRIORITY_QUEUE) {
        return;
    }

    pq->size = i + 1;
    pq->arr[i] = n;
    siftUp(pq, i);
}

Node *buildHuffmanTree(char *text) {
    int textLength = (int) strlen(text);
    if (textLength == 0) {
        return NULL;
    }

    int freq[N] = {0};
    int i;
    for (i = 0; i < textLength; i++) {
        char c = text[i];
        freq[(unsigned char)c]++;
    }

    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    pq->size = 0;
    pq->arr = (Node **) malloc(MAX_PRIORITY_QUEUE * sizeof(Node *));
    if (!pq || !pq->arr) {
        printf("Bellek tahsisi basarisiz\n");
        exit(1);
    }

    for (i = 0; i < N; i++) {
        if (freq[i] > 0) {
            Node *node = (Node *) malloc(sizeof(Node));
            if (!node) {
                printf("Bellek tahsisi basarisiz\n");
                exit(1);
            }
            node->data = (char) i;
            node->freq = freq[i];
            node->left = NULL;
            node->right = NULL;
            add(pq, node);
        }
    }

    while (pq->size != 1) {
        Node *left = poll(pq);
        Node *right = poll(pq);

        int sum = left->freq + right->freq;
        Node *node = (Node *) malloc(sizeof(Node));
        if (!node) {
            printf("Bellek tahsisi basarisiz\n");
            exit(1);
        }
        node->data = '\0';
        node->freq = sum;
        node->left = left;
        node->right = right;
        add(pq, node);
    }

    Node *root = peek(pq);
    free(pq->arr);
    free(pq);
    return root;
}

int isLeaf(Node *tree) {
    return tree->left == NULL && tree->right == NULL;
}

void encode(Node *root, char *str, char *codes[]) {
    if (root == NULL) {
        return;
    }
    if (isLeaf(root)) {
        codes[(unsigned char)root->data] = strdup(str);
        return;
    }
    char *leftStr = strdup(str);
    char *rightStr = strdup(str);
    if (!leftStr || !rightStr) {
        printf("Bellek tahsisi basarisiz\n");
        exit(1);
    }
    encode(root->left, strncat(leftStr, "0", MAX_TEXT - strlen(leftStr)), codes);
    encode(root->right, strncat(rightStr, "1", MAX_TEXT - strlen(rightStr)), codes);
    free(leftStr);
    free(rightStr);
}

void printCodes(char *codes[], FILE *outputFile) {
    int i;
    for (i = 0; i < N; i++) {
        if (codes[i] != NULL) {
            fprintf(outputFile, "%c -> %s\n", i, codes[i]);
        }
    }
}

void decode(Node *root, int *index, char *str, char *decodedText) {
    if (root == NULL) {
        return;
    }

    if (isLeaf(root)) {
        strncat(decodedText, &(root->data), 1);
        return;
    }

    (*index)++;
    if (str[*index] == '0') {
        root = root->left;
    } else {
        root = root->right;
    }
    decode(root, index, str, decodedText);
}

void preorderTraversal(Node *root, FILE *preorderFile) {
    if (root == NULL) {
        return;
    }

    fprintf(preorderFile, "%c -> %d\n", root->data, root->freq);
    if (!isLeaf(root)) {
        preorderTraversal(root->left, preorderFile);
        preorderTraversal(root->right, preorderFile);
    }
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");
    if (!inputFile) {
        printf("giriş dosyası oluşturulamıyor.\n");
        return 1;
    }

    char text[MAX_TEXT];
    fgets(text, MAX_TEXT, inputFile);
    text[strcspn(text, "\n")] = '\0'; // fgets ile gelen newline karakterini kaldır

    int textLength = (int) strlen(text);

    Node *tree = buildHuffmanTree(text);

    char *huffmanCodes[N] = {0};
    encode(tree, "", huffmanCodes);

    fclose(inputFile);

    FILE *outputFile = fopen("output.txt", "w");
    if (!outputFile) {
        printf("Çıkış dosyası oluşturulamıyor.\n");
        return 1;
    }

    fprintf(outputFile, "Huffman kodlari :\n");
    printCodes(huffmanCodes, outputFile);

    char encoded[MAX_ENCODED_TEXT] = "";
    int i;
    for (i = 0; i < textLength; i++) {
        char c = text[i];
        strcat(encoded, huffmanCodes[(unsigned char)c]);
    }

    fprintf(outputFile, "\nSifrelenen metin: %s\n", encoded);

    int encodedSize = (int) strlen(encoded);

    char decoded[MAX_TEXT] = "";
    if (isLeaf(tree)) {
        while (tree->freq > 0) {
            fprintf(outputFile, "%c", tree->data);
            tree->freq--;
        }
    } else {
        int index = -1;
        while (index < encodedSize - 1) {
            decode(tree, &index, encoded, decoded);
        }
    }
    fprintf(outputFile, "\nSifresi cozulen metin: %s\n", decoded);

    fclose(outputFile);

    // Preorder sıralamasını yap ve dosyaya yaz
    FILE *preorderFile = fopen("preorder.txt", "w");
    if (!preorderFile) {
        printf("preorder dosyası oluşturulamıyor.\n");
        return 1;
    }

    fprintf(preorderFile, "Frekanslara gore preorder siralamasi:\n");
    preorderTraversal(tree, preorderFile);

    fclose(preorderFile);
    printf("\n-------İŞLEMLER DOSYALARDA GERÇEKLEŞTİ-------\n");
    return 0;
}
