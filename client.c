#include "header.h"


int main(){
    /// getting the confusion matrix
    float **confusion_matrix = __init__mat(16);

    ///initializing the start wrapper to start making the tree
    struct Node *root;
    struct Node_wrap* start_wrap = __init__start_wrapper(&root);

    /// Starting Beam search decoding
    Beam_Search_Decoding(start_wrap, confusion_matrix);

    /// Printing Versions
    void* d;
    for(int i=0; i<9;i++){
        printf("\n PRESS ANY KEY TO CONTINUE..");
        scanf("%p", &d);
        printf("\n********************************************* VERSION %d *********************************************************************************************\n",i);
        print2D(root, i);
    }
return 0;
}
