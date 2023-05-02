#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CHILDREN 4

typedef struct Point{
    int x;
    int y;
} Point;

typedef struct MBR{
    Point top_right;
    Point bottom_left;
} MBR;

typedef struct Node_t Node;
struct Node_t{
    MBR mbr;
    int num_children;
    bool is_leaf;
    Node* children[MAX_CHILDREN];
    Point* points[MAX_CHILDREN];
};

typedef struct RTree_t RTree;
struct RTree_t{
    int M;
    Node* root;
};

Node* createNode(MBR mbr, bool is_leaf);
//creates and returns Node struct

RTree* createRTree(int M);
//creates and returns RTree struct

int max(int x, int y);
//returns max. of given two integers

int min(int x, int y);
//retruns min. of given two integers

float centerOfMBR_x(Node* node);
//returns X cordinate of center of MBR

float centerOfMBR_y(Node* node);
//returns Y cordinate of center of MBR

bool intersects(MBR rect1, MBR rect2);
//tells if rect1(rectangle) and rect2(rectangle) intrsect

bool PointIntersectsMBR(Point* p, MBR mbr);
//tells if a point and mbr(rectangle) intrsect

MBR get_MBR_leaf(Node* node);
//calculates and returns MBR for leaf nodes

MBR get_MBR(Node* node);
//calculates and returns MBR for non-leaf nodes

void merge_x(Point arr[], int l, int m, int r);
//implementation of merging function (iterative) for X cordinate sorting

void mergeSort_x(Point arr[], int l, int r);
//merge sort (recursive) algorithm for soring points by X cordinate of Point array

void merge_y(Point arr[], int l, int m, int r);
//implementation of merging function (iterative) for Y cordinate sorting

void mergeSort_y(Point arr[], int l, int r);
//merge sort (recursive) algorithm for soring points by Y cordinate of Point array

void merge_x_node(Node** arr, int l, int m, int r);
//implementation of merging function (iterative) for X cordinate sorting

void mergeSort_x_node(Node** arr, int l, int r);
//merge sort (recursive) algorithm for soring points by X cordinate of Node pointer array

void merge_y_node(Node** arr, int l, int m, int r);
//implementation of merging function (iterative) for Y cordinate sorting

void mergeSort_y_node(Node** arr, int l, int r);
//merge sort (recursive) algorithm for soring points by Y cordinate of Node pointer array

void Sort_Nodes(Node** list,int size);
//algorithm for sorting of node pointer array according to size (and slice value)

void Print_tree(Node* rootnode);
//algorithm for printing entire R-Tree

Node** InsertNode(Node** list,int size);
//algorithm as mentioned in research paper (STR) for bottom-up creation of R-Tree

RTree* insertDataSTR(Point* arr, RTree* tree,int size);
//algorithm for creating leaf nodes of R-Tree and inserting data points in them

int count_lines(char* filename);
//algorithm for returning number of lines in file (used for adding data in Point array)


Node* createNode(MBR mbr, bool is_leaf){
    Node* node = malloc(sizeof(Node));
    node->mbr = mbr;
    node->num_children = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < MAX_CHILDREN; i++){
        node->children[i] = NULL;
    }
    return node;
}

RTree* createRTree(int M){
    RTree* rtree = malloc(sizeof(RTree));
    rtree->M = M;
    MBR* mbr = (MBR*)malloc(sizeof(MBR));
    rtree->root = createNode(*mbr, true);
    return rtree;
}

bool PointIntersectsMBR(Point* p, MBR mbr){
    if((p->x > mbr.top_right.x) || (p->x < mbr.bottom_left.x)){
        return false;
    }
    else if((p->y > mbr.top_right.y) || (p->x < mbr.bottom_left.y)){
        return false;
    }
    else
        return true;
}

int max(int x, int y){
    if(x>y)
        return x;
    else
        return y;
}

int min(int x, int y){
    if(x>y)
        return y;
    else
        return x;
}

MBR get_MBR_leaf(Node* node){
    MBR mbr;
    int x_max = node->points[0]->x;
    int x_min = node->points[0]->x;
    int y_max = node->points[0]->y;
    int y_min = node->points[0]->y;
    for(int i=0; i<node->num_children; i++){
        x_max = max(x_max, node->points[i]->x);
        y_max = max(y_max, node->points[i]->y);
        x_min = min(x_min, node->points[i]->x);
        y_min = min(y_min, node->points[i]->y);
    }
    mbr.bottom_left.x = x_min;
    mbr.bottom_left.y = y_min;
    mbr.top_right.x = x_max;
    mbr.top_right.y = y_max;    
    return mbr;
}


MBR get_MBR(Node* node){
    MBR mbr;
    mbr.bottom_left.x = node->children[0]->mbr.bottom_left.x;
    mbr.bottom_left.y = node->children[0]->mbr.bottom_left.y;
    mbr.top_right.x = node->children[0]->mbr.top_right.x;
    mbr.top_right.y = node->children[0]->mbr.top_right.y;  
    for(int i=0; i<node->num_children; i++){
        mbr.top_right.x = max(mbr.top_right.x, node->children[i]->mbr.top_right.x);
        mbr.top_right.y = max(mbr.top_right.y, node->children[i]->mbr.top_right.y);
        mbr.bottom_left.x = min(mbr.bottom_left.x, node->children[i]->mbr.bottom_left.x);
        mbr.bottom_left.y = min(mbr.bottom_left.y, node->children[i]->mbr.bottom_left.y);
    }
    return mbr;
}

bool intersects(MBR rect1, MBR rect2){
    if(((rect1.bottom_left.x)>(rect2.top_right.x)) || ((rect1.top_right.x)<(rect2.bottom_left.x)))
        return false;
    else if(((rect1.top_right.y)<(rect2.bottom_left.y)) || ((rect1.bottom_left.y)>(rect2.top_right.y)))
        return false;
    else 
        return true;
}

void merge_x(Point arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    Point L[n1], R[n2];
 
    for (i = 0; i < n1; i++){
        L[i].y = arr[l + i].y;
        L[i].x = arr[l + i].x;
    }
    for (j = 0; j < n2; j++){
        R[j].y = arr[m + 1 + j].y;
        R[j].x = arr[m + 1 + j].x;
    }
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        if (L[i].x <= R[j].x){
            arr[k].y = L[i].y;
            arr[k].x = L[i].x;
            i++;
        }
        else{
            arr[k].y = R[j].y;
            arr[k].x = R[j].x;
            j++;
        }
        k++;
    }

    while (i < n1){
        arr[k].y = L[i].y;
        arr[k].x = L[i].x;
        i++;
        k++;
    }

    while (j < n2){
        arr[k].y = R[j].y;
        arr[k].x = R[j].x;
        j++;
        k++;
    }
}

void merge_y(Point arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    Point L[n1], R[n2];
 
    for (i = 0; i < n1; i++){
        L[i].y = arr[l + i].y;
        L[i].x = arr[l + i].x;
    }
    for (j = 0; j < n2; j++){
        R[j].y = arr[m + 1 + j].y;
        R[j].x = arr[m + 1 + j].x;
    }
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        if (L[i].y <= R[j].y){
            arr[k].y = L[i].y;
            arr[k].x = L[i].x;
            i++;
        }
        else{
            arr[k].y = R[j].y;
            arr[k].x = R[j].x;
            j++;
        }
        k++;
    }

    while (i < n1){
        arr[k].y = L[i].y;
        arr[k].x = L[i].x;
        i++;
        k++;
    }

    while (j < n2){
        arr[k].y = R[j].y;
        arr[k].x = R[j].x;
        j++;
        k++;
    }
}


void mergeSort_x(Point arr[], int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;
        // Sort first and second halves
        mergeSort_x(arr, l, m);
        mergeSort_x(arr, m + 1, r);
        merge_x(arr, l, m, r);
    }
}

void mergeSort_y(Point arr[], int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;
        // Sort first and second halves
        mergeSort_y(arr, l, m);
        mergeSort_y(arr, m + 1, r);
        merge_y(arr, l, m, r);
    }
}

void Print_tree(Node* rootnode){   
    if(rootnode==NULL){
        return;
    }
    if(rootnode->is_leaf==0){
        printf("Internal node; MBR values: Bottom left: (%d,%d)  Top right: (%d,%d)\n", rootnode->mbr.bottom_left.x, rootnode->mbr.bottom_left.y, rootnode->mbr.top_right.x, rootnode->mbr.top_right.y);
        for(int j=0;j<rootnode->num_children;j++){
            Print_tree(rootnode->children[j]);
        }
    }
    else if (rootnode->is_leaf==1){
        printf("External node; MBR values: Bottom left: (%d,%d)  Top right: (%d,%d)\n", rootnode->mbr.bottom_left.x, rootnode->mbr.bottom_left.y, rootnode->mbr.top_right.x, rootnode->mbr.top_right.y);
        for(int i=0;i<rootnode->num_children;i++){
            printf("Datapoints:\t (%d,%d)\n", rootnode->points[i]->x, rootnode->points[i]->y);
        }
    }
}

float centerOfMBR_x(Node* node){
    return (node->mbr.bottom_left.x+node->mbr.top_right.x)/2.0;
}

float centerOfMBR_y(Node* node){
    return (node->mbr.bottom_left.y+node->mbr.top_right.y)/2.0;
}


void merge_y_node(Node** arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    Node* L[n1];
    Node* R[n2];
 
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1 + j];
        R[j] = arr[m + 1 + j];
    }
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        if (centerOfMBR_y((L[i])) <= centerOfMBR_y(R[j])){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_x_node(Node** arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    Node* L[n1];
    Node* R[n2];
 
    for (i = 0; i < n1; i++){
        L[i] = arr[l + i];
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = arr[m + 1 + j];
        R[j] = arr[m + 1 + j];
    }
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2){
        if (centerOfMBR_x((L[i])) <= centerOfMBR_x(R[j])){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort_y_node(Node** arr, int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;
        // Sort first and second halves
        mergeSort_y_node(arr, l, m);
        mergeSort_y_node(arr, m + 1, r);
        merge_y_node(arr, l, m, r);
    }
}

void mergeSort_x_node(Node** arr, int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;
        // Sort first and second halves
        mergeSort_x_node(arr, l, m);
        mergeSort_x_node(arr, m + 1, r);
        merge_x_node(arr, l, m, r);
    }
}

void Sort_Nodes(Node** list,int size){
    int p = ceil(size/(float)MAX_CHILDREN); 
    int s = ceil(sqrt(p));
    int slice  = s*MAX_CHILDREN;
    mergeSort_x_node(list, 0, size-1);

    if(slice>=size){
        mergeSort_y_node(list, 0, size-1);
    }
    else{
        int x = floor(size/(float)slice);
        for(int i=0; i < x; i++){
            mergeSort_y_node(list, i*slice, ((i+1)*slice)-1);
        }
        mergeSort_y_node(list, x*slice, size-1);
    }

}

Node** InsertNode(Node** list,int size){ 
        if(size==1){
            return list;
        }
        Sort_Nodes(list,size);
        int p = ceil(size/(float)MAX_CHILDREN); 
        Node* listNodes[p];
    
        int extra=size%MAX_CHILDREN;
        int j;
        for(j=0; j<(size-extra);j=j+4){
            MBR mbr;
            Node* n = createNode(mbr,0);
            for(int i = 0; i < MAX_CHILDREN; i++){
                n->children[i] = list[j+i];
            }
            n->num_children=MAX_CHILDREN;
            mbr= get_MBR(n);
            n->mbr=mbr;
            listNodes[j/MAX_CHILDREN]=n;
        }
        if(extra!=0){
            MBR mbr;
            Node* n = createNode(mbr,0);
            int i=0;
            for(; i <extra; i++) {
                n->children[i] = list[j+i];
            }
            for(;i<MAX_CHILDREN;i++){
               n->children[i] = NULL;  
            }
            n->num_children=extra;
            mbr= get_MBR(n);
            n->mbr=mbr;
            listNodes[j/MAX_CHILDREN]=n;
        }
        return InsertNode(listNodes,p);
}

RTree* insertDataSTR(Point* arr, RTree* tree,int size){
    int p = ceil(size/(float)MAX_CHILDREN);
    Node* LeafNodes[p];
    int extra = size%MAX_CHILDREN;
    int i;
    for(i=0; i<size-extra; i=i+MAX_CHILDREN){
        MBR mbr;
        LeafNodes[(i/MAX_CHILDREN)] = createNode(mbr,1);
        for(int k=0;k<MAX_CHILDREN;k++){
        (LeafNodes[(i/MAX_CHILDREN)]->points[k])=&(arr[i+k]);
        }
        LeafNodes[(i/MAX_CHILDREN)]->num_children=MAX_CHILDREN;
        mbr = get_MBR_leaf(LeafNodes[(i/MAX_CHILDREN)]);
        LeafNodes[(i/MAX_CHILDREN)]->mbr=mbr;
    }
    if(extra!=0){
        MBR mbr;
        LeafNodes[(i/MAX_CHILDREN)] = createNode(mbr,1);
        for(int k=0;k<extra;k++){
            (LeafNodes[(i/MAX_CHILDREN)]->points[k])=&(arr[i+k]);
        }
        LeafNodes[(i/4)]->num_children=extra;
        mbr = get_MBR_leaf(LeafNodes[(i/MAX_CHILDREN)]);
        LeafNodes[(i/MAX_CHILDREN)]->mbr=mbr;
    }
    Node* rootnode= *(InsertNode(LeafNodes,p));  
    tree->root= rootnode;
    return tree;
}

int count_lines(char* filename){
    FILE* fp;
    int count = 0;
    char ch;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Error opening file %s\n", filename);
        return -1;
    }
    while ((ch = fgetc(fp)) != EOF){
        if (ch == '\n') {
            count++;
        }
    }
    fclose(fp);
    return count;
}


int main(void){
    printf("Enter file name which contains data:\n");
    char filename[50];
    scanf("%s", filename);
    int size = count_lines(filename);
    
    FILE* ptr;
    ptr = fopen(filename, "r");
    if (ptr == NULL) {
        printf("File can't be opened \n");
    }

    Point arr[size];
    for(int i=0;i<size;i++){
        fscanf(ptr, "%d %d", &arr[i].x, &arr[i].y);
    }
    fclose(ptr);
    
    mergeSort_x(arr, 0, size-1);

    int p = ceil(size/(float)MAX_CHILDREN);
    int s = ceil(sqrt(p));
    int slice  = s*MAX_CHILDREN;
    if(slice>=size){
        mergeSort_y(arr, 0, size-1);
    }
    else{
        int x = floor(size/(float)slice);
        for(int i=0; i < x; i++){
            mergeSort_y(arr, i*slice, ((i+1)*slice)-1);
  
        }
        mergeSort_y(arr, x*slice, size-1);
    }

    
    RTree* tree = createRTree(4);
    tree = insertDataSTR(arr, tree, size);
    Print_tree(tree->root);
}
