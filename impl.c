#include"header.h"

/// Global variable Count
int version = 0;



/// A utility function to create a new Queue node.
struct QNode* newQNode(struct Node* x)
{
    if(x==NULL) return NULL;
	struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
	temp->leaf = x;
	temp->next = NULL;
	return temp;
}

/// A utility function to create an empty queue
struct Queue* createQueue()
{
	struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	return q;
}

/// Enqueue
void enQueue(struct Queue* q, struct Node *x)
{
    if(x == NULL) return;
    if(q==NULL) return;
	struct QNode* temp = newQNode(x);

	if (q->rear == NULL) {
		q->front = q->rear = temp;
		return;
	}
	q->rear->next = temp;
	q->rear = temp;
}

/// Dequeue
struct QNode* deQueue(struct Queue* q)
{
	if (q->front == NULL)
		return NULL;

	struct QNode* temp = q->front;
    q->front = q->front->next;

	if (q->front == NULL)
		q->rear = NULL;
	return temp;
}

/// A function to print the in-order traversal of the tree
void inorder(struct Node* temp)
{
    if (!temp)
        return;

    inorder(temp->left);
    printf("%s ", temp->key);
    inorder(temp->right);
}

/// The function to create tree node
struct Node* newNode(char* key, float lg)
{
    struct Node* temp =(struct Node*)malloc(sizeof(struct Node));
    temp->key = key;
    temp->log_score = lg;
    temp->left = temp->right = NULL;
    temp->lower_ver = version;
    temp->upper_ver = 12345;
    temp->back_ptr = NULL;
    return temp;
};

/// Utility function to insert the tree node with given key value and log probability score value
struct Node* util_insert(struct Node* temp, char* key, float lg)
{
    if(temp == NULL) return NULL;
    struct Queue* q = createQueue();
    enQueue(q, temp);
    struct Node* root = temp;
    while(q->front!=NULL) {
        struct Node* temp = q->front->leaf;
        deQueue(q);

        if (!temp->left) {
            temp->left = newNode(key, lg);
            temp->left->back_ptr = temp;
            return temp->left;
        } else
            enQueue(q, temp->left);

        if (!temp->right) {
            temp->right = newNode(key,lg);
            temp->right->back_ptr = temp;
            return temp->right;
        } else
            enQueue(q, temp->right);
    }

}

/// The function which inserts the two most probable words to the corresponding tree node
struct Node_wrap* insertion(struct Node *root, float ** conf_mat){
    if(root == NULL) return NULL;
    struct wrap* pair=  most_probable(root->key, conf_mat);
    struct Node_wrap* r = (struct Node_wrap*)malloc(sizeof(struct Node_wrap));
    r->x = util_insert(root, pair->fw, pair->p1+root->log_score);
    r->y = util_insert(root, pair->sw, pair->p2+root->log_score);
    return r;
}

/// The function which gives the two words with highest log score so far on the latest level and
/// returns back the address to these two nodes to move forward with the process
struct Node_wrap* wrapper_with_best_log_score(struct Node_wrap* r1, struct Node_wrap* r2){
    if(r1 == NULL) return NULL;
    if(r2== NULL) return NULL;
    if(r1->x == NULL) return NULL;
    if(r1->y == NULL) return NULL;
    if(r2->x == NULL) return NULL;
    if(r2->y == NULL) return NULL;
    struct Node* u = r1->x;
    struct Node* v = r1->y;
    struct Node* w = r2->x;
    struct Node* x = r2->y;
    struct Node* max = u;
    if(v->log_score > max->log_score ) max =v;
    if(w->log_score > max->log_score) max = w;
    if(x->log_score > max->log_score) max= x;
    struct Node* sec_max;
    if(max!= u)sec_max = u;
    else if(max->log_score!=v->log_score)sec_max = v;
    else if(max->log_score!=w->log_score)sec_max = w;
    else if(max->log_score!=x->log_score)sec_max = x;

    if(v->log_score > sec_max->log_score && v->log_score != max->log_score) sec_max = v;
    if(w->log_score > sec_max->log_score && w->log_score != max->log_score) sec_max = w;
    if(x->log_score > sec_max->log_score && x->log_score != max->log_score) sec_max = x;

    struct Node_wrap* res = (struct Node_wrap*)malloc(sizeof(struct Node_wrap));
    res->x = max;
    res->y = sec_max;
    if(max!=u && sec_max!=u)u->upper_ver = version;
    if(max!=v && sec_max!=v)v->upper_ver = version;
    if(max!=w && sec_max!=w)w->upper_ver = version;
    if(max!=x && sec_max!=x)x->upper_ver = version;
    if(max->key == "end"){
        if(max == u){
            v->upper_ver = version;
            w->upper_ver = version;
            x->upper_ver = version;
        }
        else if(max == v){
            u->upper_ver = version;
            w->upper_ver = version;
            x->upper_ver = version;
        }
        else if(max == w){
            u->upper_ver = version;
            v->upper_ver = version;
            x->upper_ver = version;
        }
        else if(max == x){
            u->upper_ver = version;
            v->upper_ver = version;
            w->upper_ver = version;
        }
    }

    if(max->back_ptr == sec_max->back_ptr){
        struct Node* sibling;
        if(max!=u && sec_max!=u){
            sibling = get_sibling(u);
            while(sibling->upper_ver < 12345){
                u = u->back_ptr;
                u->upper_ver = version;
                sibling = get_sibling(u);
            }
        }
        else if(max!=v && sec_max!=v){
            sibling = get_sibling(v);
            while(sibling->upper_ver < 12345){
                v = v->back_ptr;
                v->upper_ver = version;
                sibling = get_sibling(v);
            }
        }
        else if(max!=w && sec_max!=w){
            sibling = get_sibling(w);
            while(sibling->upper_ver < 12345){
                w = w->back_ptr;
                w->upper_ver = version;
                sibling = get_sibling(w);
            }
        }
        else if(max!=x && sec_max!=x){
            sibling = get_sibling(x);
            while(sibling->upper_ver < 12345){
                x = x->back_ptr;
                x->upper_ver = version;
                sibling = get_sibling(x);
            }
        }
    }
    return res;
}

/// The function to recursively move forward with the process of Beam search decoding
void Beam_Search_Decoding(struct Node_wrap* r, float ** conf_mat){
    if(r == NULL) return;
    if(version==8) return;
    if(r->y==NULL){
        struct Node_wrap* r1 = insertion(r->x, conf_mat);
        version++;
        Beam_Search_Decoding(r1, conf_mat);
    }
    else{
        struct Node_wrap* r1 = insertion(r->x, conf_mat);
        struct Node_wrap* r2 = insertion(r->y, conf_mat);
        struct Node_wrap* res = wrapper_with_best_log_score(r1, r2);
        version++;
        Beam_Search_Decoding(res, conf_mat);
    }
}

/// Utility function to print the binary tree sideways
void print2DUtil(struct Node *root, int space,int ver)
{
    if (root == NULL) return;

    /// Distance between levels
    space += COUNT;
    if(root->lower_ver<= ver && root->upper_ver>=ver){
    print2DUtil(root->right, space, ver);
    }
    else return;
    printf("\n");
    for (int i = COUNT; i < space; i++)
        printf("  ");
    printf("{<%s>,%0.2f}\n", root->key, root->log_score);
    if(root->lower_ver<= ver && root->upper_ver>= ver){
    print2DUtil(root->left, space, ver);
    }
    else return;
}

/// A wrapper function of print 2D utility function
void print2D(struct Node *root, int ver)
{
    if(root == NULL) return;
    print2DUtil(root, 0, ver);
}

struct wrap* most_probable(char * word, float ** conf_mat){
    /// The 2D confusion matrix follows the following sequence of words
    /// <start> <He> <I> <hit> <struck> <was> <got> <a> <me> <tart> <pie> <with> <on> <in> <one> <end>
    int i = word_2_idx(word);
    int flag = 0;

    struct wrap* pair = (struct wrap*)malloc(sizeof(struct wrap));
    for(int j=0;j<16;j++){
        if(conf_mat[i][j]!=-8){
            if(flag==0){
                pair->fw = idx_2_word(j);
                pair->p1 = conf_mat[i][j];
                flag=1;
            }
            else if(flag==1){
                pair->sw = idx_2_word(j);
                pair->p2 = conf_mat[i][j];
                return pair;
            }
        }
    }
    /// when all log probabilities are equal
    pair->fw = idx_2_word(5);
    pair->p1 = -8;
    pair->p2 = -8;
    pair->sw = idx_2_word(8);
    return pair;
}

/// A function to print the confusion matrix
void printmatrix(float **mat){
    if(mat==NULL){
        printf("Matrix is empty \n");
        return;
    }
    float **arr = mat;
    int V = 16;
    for(int i=0; i<V; i++){
        for(int j=0; j<V; j++){
            printf("%0.2f ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/// A dictionary to give index of corresponding words int the confusion matrix
int word_2_idx(char* word){
    if(word == "start") return 0;
    if(word == "He") return 1;
    if(word == "I") return 2;
    if(word == "hit") return 3;
    if(word == "struck") return 4;
    if(word == "was") return 5;
    if(word == "got") return 6;
    if(word == "a") return 7;
    if(word == "me") return 8;
    if(word == "tart") return 9;
    if(word == "pie") return 10;
    if(word == "with") return 11;
    if(word == "on") return 12;
    if(word == "in") return 13;
    if(word == "one") return 14;
    if(word == "end") return 15;
    printf("\n Word not present in the dictionary");
    return -1;
}


/// A dictionary to give words corresponding to index in the confusion matrix
char* idx_2_word(int index){
    if(index == 0) return "start";
    if(index == 1) return "He";
    if(index == 2) return "I";
    if(index == 3) return "hit";
    if(index == 4) return "struck";
    if(index == 5) return "was";
    if(index == 6) return "got";
    if(index == 7) return "a";
    if(index == 8) return "me";
    if(index == 9) return "tart";
    if(index == 10) return "pie";
    if(index == 11) return "with";
    if(index == 12) return "on";
    if(index == 13) return "in";
    if(index == 14) return "one";
    if(index == 15) return "end";
    printf("\n Index out of Bound");
    return "\0";
}

/// A function to allocate memory to a dynamic 2D Array and create the confusion matrix;
float** __init__mat(int V){
    float **arr = (float **)calloc(V, sizeof(float*));
    if(arr==NULL){
        printf("Heap is full \n");
        exit(1);
    }
    for(int i=0; i<V; i++){
        arr[i] = (float *)calloc(V, sizeof(float));
        if(arr[i]==NULL){
            printf("Heap is full \n");
            exit(1);
        }
    }

    /// Setting the minimum log probability values to be -8
    float ** confusion_matrix = arr;
    for(int i=0; i<V; i++){
        for(int j=0; j<V; j++){
            confusion_matrix[i][j]=-8;
        }
    }

    /// feeding the data required to analyze the problem
    confusion_matrix[0][1] = -0.7;
    confusion_matrix[0][2] = -0.9;
    confusion_matrix[1][3] = -1;
    confusion_matrix[1][4] = -2.2;
    confusion_matrix[2][5] = -0.7;
    confusion_matrix[2][6] = -0.9;
    confusion_matrix[3][7] = -1.1;
    confusion_matrix[3][8] = -0.8;
    confusion_matrix[5][3] = -1.3;
    confusion_matrix[5][4] = -2.2;
    confusion_matrix[7][9] = -1.2;
    confusion_matrix[7][10] = -0.6;
    confusion_matrix[8][11] = -0.8;
    confusion_matrix[8][12] = -1;
    confusion_matrix[10][15] = -1.1;
    confusion_matrix[10][13] = -1.4;
    confusion_matrix[11][7] = -0.4;
    confusion_matrix[11][14] = -1;

    return arr;
}

/// Gets the sibling of a Tree Node "x" with the use of back_ptr
struct Node* get_sibling(struct Node* x){
    struct Node *parent = x->back_ptr;
    if(parent->left->key == x->key)return parent->right;
    else return parent->left;
}


struct Node_wrap* __init__start_wrapper(struct Node **root){

    /// Creating the start node
    *root = newNode("start", 0);

    struct Node_wrap * temp = (struct Node_wrap*)malloc(sizeof(struct Node_wrap));
    temp->x = *root;
    temp->y = NULL;
    version++;
    return temp;
}

