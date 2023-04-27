#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
1. `createRTree(int m, int M)`:
    This function creates a new RTree object with the specified minimum and maximum number of children (m and M, respectively).

2. `insertPoint(Point p, RTree* tree)`:
    This function inserts a new point into the RTree. It first finds the leaf node that should contain the point, then adds the point to the node. If the node becomes overfull as a result, it is split.

3. `findLeaf(Point p, Node* node)`:
    This function recursively searches for the leaf node that should contain a given point. It returns a pointer to the leaf node.

4. `split(Node* node)`:
    This function splits an overfull node into two new nodes. It chooses the two children with the greatest difference in MBR area to be in different nodes.

5. `insertRect(MBR rect, Node* node)`:
    This function recursively inserts a rectangle into the RTree. It first finds the leaf node that should contain the rectangle, then adds the rectangle to the node. If the node becomes overfull as a result, it is split.

6. `search(MBR rect, RTree* tree)`:
    This function searches for all rectangles in the RTree that overlap with a given rectangle. It returns a list of pointers to the rectangles.

7. `remove(MBR rect, RTree* tree)`:
    This function removes all rectangles in the RTree that overlap with a given rectangle. It first searches for the rectangles to be removed, then deletes them from their respective nodes. If a node becomes underfull as a result, it is merged with a sibling node.

8. `get_MBR(Node* node)`:
    This function computes the minimum bounding rectangle (MBR) that contains all the rectangles in a given node. It returns the MBR as a new rectangle.

9. `get_area(MBR rect)`:
    This function computes the area of a given rectangle. It returns the area as a double.

10. `compute_area(MBR rect1, MBR rect2)`:
    This function computes the area of the smallest rectangle that contains two given rectangles. It returns the area as a double.

11. `add_child(Node* node, Node* child)`:
    This function adds a child node to a given node. It updates the node's MBR to include the MBR of the child.

12. `update_mbr(Node* node)`:
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
