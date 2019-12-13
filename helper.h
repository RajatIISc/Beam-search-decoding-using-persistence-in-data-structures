#include <stdio.h>
#include <stdlib.h>
#define COUNT 10
#include <string.h>

struct wrap{
    char* fw;
    float p1;
    char* sw;
    float p2;
};

struct Node_wrap{
    struct Node* x;
    struct Node *y;
};

struct QNode {
	struct Node* leaf;
	struct QNode* next;
};

struct Queue {
	struct QNode *front, *rear;
};

struct Node {
    char* key;
    float log_score;
    int upper_ver;
    int lower_ver;
    struct Node* back_ptr;
    struct Node* left, *right;
};

struct QNode* newQNode(struct Node* x);
struct Queue* createQueue();
void enQueue(struct Queue* q, struct Node *x);
struct QNode* deQueue(struct Queue* q);
void inorder(struct Node* temp);
struct Node* newNode(char* key, float lg);
struct Node* util_insert(struct Node* temp, char* key, float lg);
void print2DUtil(struct Node *root, int space, int ver);
struct wrap* most_probable(char * word, float ** conf_mat);
int word_2_idx(char* word);
char* idx_2_word(int index);
float** __init__mat(int V);
void printmatrix(float **mat);
struct Node_wrap* insertion(struct Node *root, float ** conf_mat);
struct Node_wrap* wrapper_with_best_log_score(struct Node_wrap* r1, struct Node_wrap* r2);
struct Node* get_sibling(struct Node* x);


