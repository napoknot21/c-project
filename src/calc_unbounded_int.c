#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


#define LOWER_CASE 97
#define UPPER_CASE 65

/**
 * Variables storage node.
 */
typedef struct node node;
/**
 * Variables storage tree.
 */
typedef struct tree tree;

node *create_node(char id, int data);

tree *create_tree();

int max(int a, int b);

int min(int a, int b);

void node_free(node *n);

void tree_free(tree *t);

int getPos(char c);

int isUpperCase(char c);

int isLowerCase(char c);

int node_add(node *pNode, int n, char *string);

int node_getValue(node *pNode, char *string);

int node_size(node *pNode);

int isLetter(char c);

/**
 * Variables storage node.
 */
struct node {
    char id;
    int data;
    node *children[52];
    int length;
};

/**
 * Variables storage tree.
 */
struct tree {
    node *children[52];
    int length;
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
tree *create_tree() {
    tree *t = malloc(sizeof(tree));
    if (t == NULL) return NULL;
    t->length = 0;
    return t;
}

/**
 * Create a node in a tree.
 *
 * @param id the node's id
 * @param data the node's data.
 * @return the pointer of the created node.
 */
node *create_node(char id, int data) {
    node *n = malloc(sizeof(node));
    if (n == NULL) {
        return NULL;
    }
    n->id = id;
    n->data = data;
    n->length = 0;
    return n;
}

/**
 * Gets the value of the given variable.
 *
 * @param t The storage tree.
 * @param string The variable's name
 * @return the data value if the value is found, -1 otherwise
 */
int tree_getValue(tree *t, char *string) {
    int slen = (int) strlen(string);
    if (string == NULL || slen < 1) return -1;
    for (int i = 0; i < t->length; i++) {
        if (t->children[i]->id == string[0]) {
            return node_getValue(t->children[i], &string[1]);
        }
    }
    return 0;
}

/**
 * Gets the value of the given variable.
 *
 * @param n The current node.
 * @param string The variable's name.
 * @return the data value if the value is found, -1 otherwise.
 */
int node_getValue(node *n, char *string) {
    int slen = (int) strlen(string);
    if (slen == 0) return (n == NULL) ? -1 : n->data;
    for (int i = 0; i < n->length; i++) {
        if (n->children[i]->id == string[0]) {
            return node_getValue(n->children[i], &string[1]);
        }
    }
    return -1;
}

/**
 * Add a variable in the tree, if the variable is already present, its data will be updated.
 *
 * @param t the storage tree.
 * @param n the data value.
 * @param string the variable's name.
 * @return   0 if the variable was normally added. <br>
 *          -1 if the name is incorrect. <br>
 *          -2 if the tree is NULL. <br>
 *          -3 if the variable wasn't found
 */
int tree_add(tree *t, int n, char *string) {
    if (t == NULL) return -2;
    int slen = (int) strlen(string);
    if (string == NULL || slen < 1) return -1;
    for (int i = 0; i < t->length; i++) {
        if (t->children[i]->id == string[0]) {
            return node_add(t->children[i], n, &string[1]);
        }
    }
    if (t->length > 52) return -1;
    if (slen == 1) {
        t->children[t->length] = create_node(string[0], n);
    } else {
        t->children[t->length] = create_node(string[0], 0);
        node_add((t->children[t->length]), n, &string[1]);

    }
    t->length++;
    return 0;
}

/**
 * Add a variable in the tree, if the variable is already present, its data will be updated.
 *
 * @param pNode the current node.
 * @param n the data value.
 * @param string the variable's name.
 * @return  0 if the variable was normally added. <br>
 *          -3 if the variable wasn't found
 */
int node_add(node *pNode, int n, char *string) {
    int slen = (int) strlen(string);
    if (slen == 0) {
        pNode->data = n;
        return 0;
    }

    for (int i = 0; i < pNode->length; i++) {
        if (pNode->children[i]->id == string[0]) {
            return node_add(pNode->children[i], n, &string[1]);
        }
    }
    if (pNode->length > 52) return -3;
    if (slen == 1) {
        pNode->children[pNode->length] = create_node(string[0], n);
    } else {
        pNode->children[pNode->length] = create_node(string[0], 0);
        node_add((pNode->children[pNode->length]), n, &string[1]);

    }
    pNode->length++;
    return 0;
}

/**
 * Calculates the size of the tree.
 *
 * @param t The storage tree.
 * @return the size of the tree.
 */
int tree_size(tree *t) {
    if (t == NULL) return 0;
    int size = 0;
    for (int i = 0; i < t->length; i++) {
        size += node_size(t->children[i]);
    }
    return size;
}

/**
 * Calculate the size of the sub-tree.
 *
 * @param pNode the root of the sub-tree.
 * @return the size of the sub-tree.
 */
int node_size(node *pNode) {
    int size = 0;
    for (int i = 0; i < pNode->length; i++) {
        size += node_size(pNode->children[i]);
    }
    printf("\n Deleting node: %c", pNode->id);
    return ++size;
}

/**
 * free the sub-tree.
 *
 * @param n the sub-tree's root.
 */
void node_free(node *n) {
    if (n == NULL) return;
    for (int i = 0; i < n->length; i++) {
        node_free(n->children[i]);
    }
    printf("\n Deleting node: %c", n->id);
    free(n);
}

/**
 * Free the storage tree.
 *
 * @param t the storage tree.
 */
void tree_free(tree *t) {
    if (t == NULL) return;
    for (int i = 0; i < t->length; i++) {
        node_free(t->children[i]);
    }
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


