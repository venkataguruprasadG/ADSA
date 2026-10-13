#include <iostream>
using namespace std;

struct Node
{
    int keys[3];    // keys[3] — stores up to three values
    Node *child[4]; // child[4] — stores up to four child pointers
    int n;          // number of keys currently in the node
    bool leaf;      // true if the node is a leaf, false otherwise
};

Node *createNode(bool leaf)
{
    Node *newNode = new Node;
    newNode->n = 0;
    newNode->leaf = leaf;
    for (int i = 0; i < 4; i++)
    {
        newNode->child[i] = nullptr;
    }

    return newNode;
}

int main()
{

    Node *root = createNode(true);
    root->keys[0] = 10;
    root->keys[1] = 20;
    root->keys[2] = 30;
    root->n = 3;
    root->leaf = true;

    if (root->n == 3)
    {
        Node *newRoot = createNode(false);
        newRoot->child[0] = root;
        newRoot->keys[0] = root->keys[1]; // Promote the middle key to the new root
        newRoot->n = 1;

        Node *rightChild = createNode(true);
        rightChild->keys[0] = root->keys[2];
        rightChild->n = 1;
        newRoot->child[1] = rightChild;

        root->n = 1;
        root = newRoot;
    }
}