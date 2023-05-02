#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CHILDREN 4
#define MIN_CHILDREN 2

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct MBR {
    Point top_right;
    Point bottom_left;
} MBR;

typedef struct Node_t Node;
struct Node_t {
    MBR mbr;//this has to be an array MBR mbr[4]
    int num_children;
    bool is_leaf;
    Node* children[MAX_CHILDREN];
    //Node* parent;//UPDATED !!
    Point* points[MAX_CHILDREN];
};

typedef struct RTree_t RTree;
struct RTree_t {
    int m;
    int M;
    int height;
    Node* root;
};

/*
UPDATED DONE `createRTree(int m, int M)`:
    This function creates a new RTree object with the specified minimum and maximum number of children (m and M, respectively).
MOST IMP. (NOT IMPLEMENTED YET) `insertPoint(Point p, RTree* tree)`:
    This function inserts a new point into the RTree. It first finds the leaf node that should contain the point, then adds the point to the node. If the node becomes overfull as a result, it is split.
DONE `findLeaf(Point p, Node* node)`:
    This function recursively searches for the leaf node that should contain a given point. It returns a pointer to the leaf node.
    PointIntersectsMBR()
MOST IMP. (NOT IMPLEMENTED YET) `split(Node* node)`:
    This function splits an overfull node into two new nodes. It chooses the two children with the greatest difference in MBR area to be in different nodes.
doesn't seem imp. (not implemented) `insertRect(MBR rect, Node* node)`:
    This function recursively inserts a rectangle into the RTree. It first finds the leaf node that should contain the rectangle, then adds the rectangle to the node. If the node becomes overfull as a result, it is split.
DONE `search(MBR rect, RTree* tree)`:
    This function searches for all rectangles in the RTree that overlap with a given rectangle. It returns a list of pointers to the rectangles.
    search_helper()... , intersects()
7. `remove(MBR rect, RTree* tree)`:
    This function removes all rectangles in the RTree that overlap with a given rectangle. It first searches for the rectangles to be removed, then deletes them from their respective nodes. If a node becomes underfull as a result, it is merged with a sibling node.
DONE `get_MBR(Node* node)`:
    This function computes the minimum bounding rectangle (MBR) that contains all the rectangles in a given node. It returns the MBR as a new rectangle.
DONE `get_area(MBR rect)`:
    This function computes the area of a given rectangle. It returns the area as a double.
DONE `compute_area(MBR rect1, MBR rect2)`:
    This function computes the area of the smallest rectangle that contains two given rectangles. It returns the area as a double.
UPDATED DONE `add_child(Node* node, Node* child)`:
    This function adds a child node to a given node. It updates the node's MBR to include the MBR of the child.
UPDATED DONE `update_mbr(Node* node)`:
    This function updates a node's MBR to include the MBRs of all its children.
DONE `adjust_tree(Node* node, Node* child)`:
    This function adjusts the RTree after a node has been split. It first adds the new child node to the parent node, then updates the MBRs of the parent and all its ancestors, possibly splitting more nodes if necessary.
*/


Node* createNode(MBR mbr, bool is_leaf) {
    Node* node = malloc(sizeof(Node));
    node->mbr = mbr;
    node->num_children = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

RTree* createRTree(int m, int M) {
    RTree* rtree = malloc(sizeof(RTree));
    rtree->m = m;
    rtree->M = M;
    rtree->height = 1;
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

// findLeaf(Point p, Node* node){
//     if ((node->is_leaf) && (PointIntersectsMBR(p, node->mbr))) {
//         return node;
//     }
//     Node* child;
//     for (int i = 0; i < node->num_children; i++) {
//         child = node->children[i];
//         if (PointIntersectsMBR(p, child->mbr)) {
//             return findLeaf(p, child);
//         }
//     }
//     return NULL;
// }

int compute_area(MBR rect1, MBR rect2){
    int x = (rect1.top_right.y - rect1.bottom_left.y)*(rect1.bottom_left.x - rect1.top_right.x);
    int y = (rect2.top_right.y - rect2.bottom_left.y)*(rect2.bottom_left.x - rect2.top_right.x);
    if(x >= y)
        return y;
    else 
        return x;
}

int get_area(MBR rect){
    int x = (rect.top_right.y - rect.bottom_left.y)*(rect.bottom_left.x - rect.top_right.x);
    return x;
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
    int x_max, x_min, y_max, y_min;
    for(int i=0; i<node->num_children-1; i++){
        x_max = max(node->points[i]->x, node->points[i+1]->x);
        y_max = max(node->points[i]->y, node->points[i+1]->y);
        x_min = min(node->points[i]->x, node->points[i+1]->x);
        y_min = min(node->points[i]->y, node->points[i+1]->y);
    }
    mbr.bottom_left.x = x_min;
    mbr.bottom_left.y = y_min;
    mbr.top_right.x = x_max;
    mbr.top_right.y = y_max;    
    return mbr;
}

MBR get_MBR(Node* node){
    MBR mbr = node->children[0]->mbr;
    for(int i=0; i<node->num_children; i++){
        mbr.top_right.x = max(mbr.top_right.x, node->children[i]->mbr.top_right.x);
        mbr.top_right.y = max(mbr.top_right.y, node->children[i]->mbr.top_right.y);
        mbr.bottom_left.x = min(mbr.bottom_left.x, node->children[i]->mbr.bottom_left.x);
        mbr.bottom_left.y = min(mbr.bottom_left.y, node->children[i]->mbr.bottom_left.y);
    }
    return mbr;
}

void update_mbr(Node* node){
    node->mbr = node->children[0]->mbr;
    for(int i=1; i<node->num_children; i++){
        node->mbr.top_right.x = max(node->mbr.top_right.x, node->children[i]->mbr.top_right.x);
        node->mbr.top_right.y = max(node->mbr.top_right.y, node->children[i]->mbr.top_right.y);
        node->mbr.bottom_left.x = min(node->mbr.bottom_left.x, node->children[i]->mbr.bottom_left.x);
        node->mbr.bottom_left.y = min(node->mbr.bottom_left.y, node->children[i]->mbr.bottom_left.y);
    }
}

Node* split(Node* node){
}

/*
void add_child(Node* node, Node* child) {
    if (node->num_children >= MAX_CHILDREN) {
        Node* temp = split(node);
        add_child(node->parent, temp);
    }
    node->children[node->num_children] = child;
    node->num_children++;
    node->mbr = get_MBR(node);
    if (node->parent != NULL) {
        update_mbr(node->parent);
    }
}
*/

bool intersects(MBR rect1, MBR rect2){
    if(((rect1.bottom_left.x)>(rect2.top_right.x)) || ((rect1.top_right.x)<(rect2.bottom_left.x)))
        return false;
    else if(((rect1.top_right.y)<(rect2.bottom_left.y)) || ((rect1.bottom_left.y)>(rect2.top_right.y)))
        return false;
    else 
        return true;
}

void merge_x(Point arr[], int l, int m, int r)
{
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
    while (i < n1 && j < n2) {
        if (L[i].x <= R[j].x) {
            arr[k].y = L[i].y;
            arr[k].x = L[i].x;
            i++;
        }
        else {
            arr[k].y = R[j].y;
            arr[k].x = R[j].x;
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k].y = L[i].y;
        arr[k].x = L[i].x;
        i++;
        k++;
    }

    while (j < n2) {
        arr[k].y = R[j].y;
        arr[k].x = R[j].x;
        j++;
        k++;
    }
}

void merge_y(Point arr[], int l, int m, int r)
{
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
    while (i < n1 && j < n2) {
        if (L[i].y <= R[j].y) {
            arr[k].y = L[i].y;
            arr[k].x = L[i].x;
            i++;
        }
        else {
            arr[k].y = R[j].y;
            arr[k].x = R[j].x;
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k].y = L[i].y;
        arr[k].x = L[i].x;
        i++;
        k++;
    }

    while (j < n2) {
        arr[k].y = R[j].y;
        arr[k].x = R[j].x;
        j++;
        k++;
    }
}


void mergeSort_x(Point arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort_x(arr, l, m);
        mergeSort_x(arr, m + 1, r);
 
        merge_x(arr, l, m, r);
    }
}

void mergeSort_y(Point arr[], int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort_y(arr, l, m);
        mergeSort_y(arr, m + 1, r);
 
        merge_y(arr, l, m, r);
    }
}

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
} List;

// List* list_create() {
//     List* list = (List*) malloc(sizeof(List));
//     list->head = NULL;
//     list->tail = NULL;
//     return list;
// }

// void list_append(List* list, void* data) {
//     ListNode* node = (ListNode*) malloc(sizeof(ListNode));
//     node->data = data;
//     node->next = NULL;
//     if (list->tail != NULL) {
//         list->tail->next = node;
//         list->tail = node;
//     } else {
//         list->head = node;

//         list->tail = node;
//     }
// }

void search_helper(MBR rect, Node* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->num_children; i++) {
        if (intersects(rect, node->children[i]->mbr)) {
            if (node->children[i]->is_leaf) {
                printf("External node; MBR values: Bottom left: (%d,%d)  Top right: (%d,%d)\n", node->mbr.bottom_left.x, node->mbr.bottom_left.y, node->mbr.top_right.x, node->mbr.top_right.y);
                
                for (int j = 0; j < node->children[i]->num_children; j++) {
                    if(PointIntersectsMBR(node->children[i]->points[j], rect)){
                        printf("Datapoints:\t (%d,%d)\n", node->children[i]->points[j]->x, node->children[i]->points[j]->y);
                    }
                }
            } 
            else {
                printf("Internal node; MBR values: Bottom left: (%d,%d)  Top right: (%d,%d)\n", node->mbr.bottom_left.x, node->mbr.bottom_left.y, node->mbr.top_right.x, node->mbr.top_right.y);
                search_helper(rect, node->children[i]);
            }
        }
    }
}

void search(MBR rect, RTree* tree) {
    // List* result_list = list_create();
    search_helper(rect, tree->root);
    // return result_list;
}

void print_tree(RTree* tree){
    MBR temp;
    temp.top_right.x = tree->root->mbr.top_right.x;
    temp.top_right.y = tree->root->mbr.top_right.y;
    temp.bottom_left.x = tree->root->mbr.bottom_left.x;
    temp.bottom_left.y = tree->root->mbr.bottom_left.y;
    search(temp, tree);
}

// void adjust_tree(Node* node, Node* child){
//     add_child(node, child);
//     Node* current = node;

//     if (current && (current->num_children > MAX_CHILDREN)) {
//         Node* new = split(current);
//         adjust_tree(node->parent, new);
//     }
// }
void Print_tree(Node* rootnode)
{   
    if(rootnode==NULL)
    {
        return;
    }
    if(rootnode->is_leaf==0)
    {
        //
        printf("Internal node; MBR values: Bottom left: (%d,%d)  Top right: (%d,%d)\n", rootnode->mbr.bottom_left.x, rootnode->mbr.bottom_left.y, rootnode->mbr.top_right.x, rootnode->mbr.top_right.y);
        for(int j=0;j<rootnode->num_children;j++)
        {
        Print_tree(rootnode->children[j]);
        }
    }
    else if (rootnode->is_leaf==1)
    {
        printf("Internal node; MBR values: Bottom left: (%d,%d)  Top right: (%d,%d)\n", rootnode->mbr.bottom_left.x, rootnode->mbr.bottom_left.y, rootnode->mbr.top_right.x, rootnode->mbr.top_right.y);
        for(int i=0;i<rootnode->num_children;i++)
        {
            printf("Datapoints:\t (%d,%d)\n", rootnode->points[i]->x, rootnode->points[i]->y);
        }
    }

}

Node** InsertNode(Node** list,int size) 
    { 
        if(size==1)
        {
            return list;
        }
        int p = ceil(size/(float)MAX_CHILDREN);
        Node* listNodes[p];
        //Node** listNodes=(Node**)malloc(sizeof(Node*)*p);
       // int size = sizeof(list)/sizeof(Node*);
        int extra=size%4;
        int j;
        for(j=0; j<(size-extra);j=j+4)
        {
            MBR mbr;
            Node* n = createNode(mbr,0);
            for(int i = 0; i < MAX_CHILDREN; i++) 
            {
                n->children[i] = list[j+i];
            }
            n->num_children=4;
            mbr= get_MBR(n);
            n->mbr=mbr;
            listNodes[j/4]=n;
        }
        if(extra!=0)
        {
            MBR mbr;
            Node* n = createNode(mbr,0);
            int i;
            for(i = 0; i <extra; i++) 
            {
                n->children[i] = list[j+i];
            }
            for(i;i<4;i++)
            {
               n->children[i] = NULL;  
            }
            n->num_children=extra;
            mbr= get_MBR(n);
            n->mbr=mbr;
            listNodes[j/4]=n;
        }
         return InsertNode(listNodes,p);
    }

RTree* insertDataSTR(Point* arr, RTree* tree,int size)
{
    int p = ceil(size/(float)MAX_CHILDREN);
    Node* LeafNodes[p];
    // int no_of_points=sizeof(arr)/sizeof(arr[0]);
    //Node** LeafNodes=(Node**)malloc(sizeof(Node*)*p);
    int extra = size%MAX_CHILDREN;
    int i;
    for(i=0; i<size-extra; i=i+4)
    {
        MBR mbr;
        LeafNodes[(i/4)] = createNode(mbr,1);
        for(int k=0;k<4;k++)
        {
        (LeafNodes[(i/4)]->points[k])=&(arr[i+k]);
        }
        LeafNodes[(i/4)]->num_children=4;
        mbr = get_MBR_leaf(LeafNodes[(i/4)]);
        LeafNodes[(i/4)]->mbr=mbr;
    }
    if(extra!=0)  
    {
        MBR mbr;
        LeafNodes[(i/4)] = createNode(mbr,1);
        for(int k=0;k<extra;k++)
        {
            (LeafNodes[(i/4)]->points[k])=&(arr[i+k]);
        }
        LeafNodes[(i/4)]->num_children=extra;
        mbr = get_MBR_leaf(LeafNodes[(i/4)]);
        LeafNodes[(i/4)]->mbr=mbr;
    }
    Node* rootnode= *(InsertNode(LeafNodes,p));  
    tree->root= rootnode;
    return tree;
}

int count_lines(char* filename) {
    FILE* fp;
    int count = 0;
    char ch;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        return -1;
    }
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    fclose(fp);
    return count;
}


int main(void){
    FILE* ptr;
    char* filename = "data.txt";
    char str[50];
    ptr = fopen(filename, "r");
    int size = count_lines(filename) + 1;
    //data.txt - 21
    //s1data1lac - 105000
 
    if (ptr == NULL) {
        printf("File can't be opened \n");
    }

    Point arr[size];
    for(int i=0;i<size;i++){
        fscanf(ptr, "%d %d", &arr[i].x, &arr[i].y);
    }

    // printf("Done\n");

    fclose(ptr);
    mergeSort_x(arr, 0, size-1);

    // printf("Now sorting\n");
    /*
    for(int i=0;i<size;i++){
        if(i%1000 == 0)
        printf("%d %d\n", arr[i].x, arr[i].y);
    }
    */

    mergeSort_y(arr, 0, size-1);
    
    /*
    printf("Now sorting\n");
    for(int i=0;i<size-1;i++){
        // printf("%d %d\n", arr[i].x, arr[i].y);
        if(arr[i].y > arr[i+1].y){
        printf("NO!! (%d)\n\n", i+1);
        break;
        }
    }
    */
    int p = ceil(size/(float)MAX_CHILDREN);
    int s = ceil(sqrt(p));
    int slice  = s*MAX_CHILDREN;
    if(slice<=size){
        mergeSort_y(arr, 0, size-1);
    }
    else{
        int x = floor(size/(float)slice);
        for(int i=0; i < x-1; i++){
            mergeSort_y(arr, i*slice, ((i+1)*slice)-1);
        }
        mergeSort_y(arr, x*slice, size-1);
    }

    RTree* tree = createRTree(2,4);
    tree = insertDataSTR(arr, tree, size);
    Print_tree(tree->root);
    //print_tree(tree);
    
    //MBR TESTING BELOW, DO NOT ENTER
    
    /*
    MBR test1;
    MBR test2;
    printf("Enter x cordinate of bottom left point of rectangle 1:\n");
    scanf("%d", &test1.bottom_left.x);
    printf("Enter y cordinate of bottom left point of rectangle 1:\n");
    scanf("%d", &test1.bottom_left.y);
    printf("Enter x cordinate of top right point of rectangle 1:\n");
    scanf("%d", &test1.top_right.x);
    printf("Enter y cordinate of top right point of rectangle 1:\n");
    scanf("%d", &test1.top_right.y);
    printf("Enter x cordinate of bottom left point of rectangle 2:\n");
    scanf("%d", &test2.bottom_left.x);
    printf("Enter y cordinate of bottom left point of rectangle 2:\n");
    scanf("%d", &test2.bottom_left.y);
    printf("Enter x cordinate of top right point of rectangle 2:\n");
    scanf("%d", &test2.top_right.x);
    printf("Enter y cordinate of top right point of rectangle 2:\n");
    scanf("%d", &test2.top_right.y);
    printf("Rectangles intersect? %d\n", intersects(test1, test2));
    */
    
}
