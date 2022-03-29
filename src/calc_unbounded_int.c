#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


#define LOWER_CASE 97
#define UPPER_CASE 65

/**
 * Variables storage node.
 */
typedef struct Node Node;
/**
 * Variables storage tree.
 */
typedef struct Tree Tree;

Node *create_node(char id, int data);

Tree *create_tree();

int max(int a, int b);

int min(int a, int b);

void node_free(Node *n);

void tree_free(Tree *t);

int getPos(char c);

int isUpperCase(char c);

int isLowerCase(char c);

int node_add(Node **pNode, int n, const char *string);

int node_getValue(Node **pNode, char *string);

int node_size(Node *pNode);

int isLetter(char c);

/**
 * Variables storage node.
 */
struct Node {
    char id;
    int data;
    Node* left;
    Node * middle;
    Node * right;
};

/**
 * Variables storage tree.
 */
struct Tree {
    Node * root;
};

//todo : Clean code
//todo : Adapt code to unbounded_int

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int min(int a, int b) {
    return (a < b) ? a : b;
}

static int isUpperCase(char c) {
    return (UPPER_CASE <= (int) c && (int) c >= UPPER_CASE + 26);
}

static int isLowerCase(char c) {
    return (LOWER_CASE < (int) c && (int) c > LOWER_CASE + 26);
}

static int isLetter(char c) {
    return isUpperCase(c) || isLowerCase(c);
}

int getPos(char c) {
    if (isUpperCase(c)) {
        return 26 + (int) c - UPPER_CASE;
    }
    if (isLowerCase(c)) {
        return (int) c - LOWER_CASE;
    }
    return -1;
}

/**
 * Create the storage tree
 * @return an empty tree.
 */
Tree * create_tree() {
    Tree* t = malloc(sizeof(Tree));
    if (t == NULL) return NULL;
    t->root = NULL;
    return t;
}

/**
 * Create a node in a tree.
 *
 * @param id the node's id
 * @param data the node's data.
 * @return the pointer of the created node.
 */
Node* create_node(char id, int data) {
    Node* n = malloc(sizeof(Node));
    if (n == NULL) {
        return NULL;
    }
    n->id = id;
    n->data = data;
    n->left = n->middle = n->right = NULL;
    return n;
}

/**
 * Gets the value of the given variable.
 *
 * @param t The storage tree.
 * @param string The variable's name
 * @return the data value if the value is found. <br>
 *          -1 if the value wasn't found. <br>
 *          -2 if the tree's integrity is compromised.
 */
int tree_getValue(Tree* t, char * string) {
    int len = (int)strlen(string);
    if (len < 1) return -1;
    return node_getValue(&t->root, string);;
}

/**
 * Gets the value of the given variable.
 *
 * @param n The current node.
 * @param string The variable's name.
 * @return the data value if the value is found. <br>
 *          -1 if the value wasn't found. <br>
 *          -2 if the tree's integrity is compromised.
 */
int node_getValue(Node **n, char *string) {
    int len = (int)strlen(string);
    if (len < 1 ) return -1;
    if (n == NULL) return -2;
    if (string[0] < (*n)->id) {
        return node_getValue(&(*n)->left,string);
    }
    if (string[0] > (*n)->id) {
        return node_getValue(&(*n)->right,string);
    }
    if (len == 1) {
        return (*n)->data;
    }
    return node_getValue(&(*n)->middle,string + 1);
}

/**
 * Add a variable in the tree, if the variable is already present, its data will be updated.
 *
 * @param t the storage tree.
 * @param n the data value.
 * @param string the variable's name.
 * @return   0 if the variable was normally added. <br>
 *          -1 if the name is empty. <br>
 *          -2 if the tree is NULL.
 */
int tree_add(Tree* t, int n, char * string) {
    if (t == NULL) return -2;
    int len = (int)strlen(string);
    if (len < 1) return -1;
    node_add(&t->root,n,string);
    return 0;
}

/**
 * Add a variable in the tree, if the variable is already present, its data will be updated.
 *
 * @param pNode the current node.
 * @param n the data value.
 * @param string the variable's name.
 * @return  0 if the variable was normally added. <br>
 *          -1 if the variable's name is empty
 */
int node_add(Node **pNode, const int n, const char *string) {
    int len = (int)strlen(string);
    if (len < 1) return -1;
    if (*pNode == NULL) {
        *pNode = create_node(string[0],0);
    }
    if (string[0] < (*pNode)->id) {
        return node_add(&(*pNode)->left,n,string);
    }
    if (string[0] > (*pNode)->id) {
        return node_add(&(*pNode)->right,n,string);
    }
    if (len > 1) {
        return node_add(&(*pNode)->middle, n, string + 1);
    }
    (*pNode)->data = n;
    return 0;
}

/**
 * Calculates the size of the tree.
 *
 * @param t The storage tree.
 * @return the size of the tree.
 */
int tree_size(Tree *t) {
    if (t->root == NULL) return 0;
    return node_size(t->root);
}

/**
 * Calculate the size of the sub-tree.
 *
 * @param pNode the root of the sub-tree.
 * @return the size of the sub-tree.
 */
int node_size(Node *n) {
    if (n == NULL) return 0;
    int size = 0;
    size += node_size(n->left);
    size += node_size(n->middle);
    size += node_size(n->right);
    return ++size;
}

/**
 * free the sub-tree.
 *
 * @param n the sub-tree's root.
 */
void node_free(Node * n) {
    if (n == NULL) return;
    node_free(n->left);
    node_free(n->middle);
    node_free(n->right);
    free(n);
}

/**
 * Free the storage tree.
 *
 * @param t the storage tree.
 */
void tree_free(Tree * t) {
    node_free(t->root);
    free(t);
}

/**
 * Some tests
 */
int main() {
    printf("Hello, World!\n");
    tree *t = create_tree();
    tree_add(t, 10243, "abc");
    printf("t(abc) = %d \n", tree_getValue(t, "abc"));
    tree_add(t, 1552, "abc");
    printf("t(abc) = %d \n", tree_getValue(t, "abc"));
    tree_add(t, 25320, "abcd");
    printf("t(abcd) = %d \n", tree_getValue(t, "abcd"));
    tree_add(t, 15420, "abcgzeqg");
    printf("t(abcgzeqg) = %d \n", tree_getValue(t, "abcgzeqg"));
    tree_add(t, 15425, "abcqggqg");
    printf("t(abc) = %d \n", tree_getValue(t, "abcqggqg"));
    tree_add(t, 254240, "abcdrqgr");
    printf("t(abcdrqgr) = %d \n", tree_getValue(t, "abcdrqgr"));
    tree_add(t, 14520, "abcqrg");
    printf("t(abcqrg) = %d \n", tree_getValue(t, "abcqrg"));
    tree_add(t, 15425, "qrgrabc");
    printf("t(qrgrabc) = %d \n", tree_getValue(t, "qrgrabc"));
    tree_add(t, 24520, "abgqrcd");
    printf("t(abgqrcd) = %d \n", tree_getValue(t, "abgqrcd"));
    tree_add(t, 24, "cddz");
    printf("t(cddz) = %d \n", tree_getValue(t, "cddz"));
    tree_add(t, 24, "cddz");
    printf("size : %d", tree_size(t));
    tree_free(t);
    return 0;
}


