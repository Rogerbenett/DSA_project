#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    MBR mbr;
    int num_children;
    bool is_leaf;
    Node* children[MAX_CHILDREN];
};

typedef struct RTree_t RTree;
struct RTree_t {
    int m;
    int M;
    int height;
    Node* root;
};

/*
DONE `createRTree(int m, int M)`:
    This function creates a new RTree object with the specified minimum and maximum number of children (m and M, respectively).

2. `insertPoint(Point p, RTree* tree)`:
    This function inserts a new point into the RTree. It first finds the leaf node that should contain the point, then adds the point to the node. If the node becomes overfull as a result, it is split.

3. `findLeaf(Point p, Node* node)`:
    This function recursively searches for the leaf node that should contain a given point. It returns a pointer to the leaf node.

4. `split(Node* node)`:
    This function splits an overfull node into two new nodes. It chooses the two children with the greatest difference in MBR area to be in different nodes.

5. `insertRect(MBR rect, Node* node)`:
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

DONE `add_child(Node* node, Node* child)`:
    This function adds a child node to a given node. It updates the node's MBR to include the MBR of the child.

DONE `update_mbr(Node* node)`:
    This function updates a node's MBR to include the MBRs of all its children.

13. `adjust_tree(Node* node, Node* child)`:
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
    rtree->root = createNode((MBR){{0, 0}, {0, 0}}, true);
    return rtree;
}

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

split(Node* node){

}

void add_child(Node* node, Node* child){
    if(node->num_children >= 4){
        split(node);
        //split
    }
    else{
        node->children[node->num_children] = child;
        node->num_children++;
        node->mbr = get_MBR(node);
    }
}

bool intersects(MBR rect1, MBR rect2){
    if(((rect1.bottom_left.x)>(rect2.top_right.x)) || ((rect1.top_right.x)<(rect2.bottom_left.x)))
        return false;
    else if(((rect1.top_right.y)<(rect2.bottom_left.y)) || ((rect1.bottom_left.y)>(rect2.top_right.y)))
        return false;
    else 
        return true;
}

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct List {
    ListNode* head;
    ListNode* tail;
} List;

List* list_create() {
    List* list = (List*) malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void list_append(List* list, void* data) {
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));
    node->data = data;
    node->next = NULL;
    if (list->tail != NULL) {
        list->tail->next = node;
        list->tail = node;
    } else {
        list->head = node;
        list->tail = node;
    }
}

void search_helper(MBR rect, Node* node, List* result_list) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < node->num_children; i++) {
        if (intersects(rect, node->children[i]->mbr)) {
            if (node->children[i]->is_leaf) {
                for (int j = 0; j < node->children[i]->num_children; j++) {
                    if (intersects(rect, node->children[i]->children[j]->mbr)) {
                        list_append(result_list, node->children[i]->children[j]);
                    }
                }
            } 
            else {
                search_helper(rect, node->children[i], result_list);
            }
        }
    }
}

List* search(MBR rect, RTree* tree) {
    List* result_list = list_create();
    search_helper(rect, tree->root, result_list);
    return result_list;
}

/*
int main(void){
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
}
*/
