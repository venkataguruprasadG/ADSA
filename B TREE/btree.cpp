#include <iostream>
#include <vector>
using namespace std;

class BTreeNode
{
public:
    vector<int> keys; // stores the values inside a node

    // keys = {20,40,60}

    vector<BTreeNode *> children; // stores pointers to child nodes

    /*   [20]
         /  \
       /      \
    [10]      [30] */

    // children = {left_child, right_child}

    bool leaf; // TELLS Whether the node is a leaf

    BTreeNode(bool isLeaf)
    {
        leaf = isLeaf;
    }
};

class Btree
{
public:
    BTreeNode *root; // stores address of the root node

    int t; // represents minimum degree

    Btree()
    {
        root = new BTreeNode(true);

        t = 2;
    }
};

void search(BTreeNode *node, int key) // * node - the node where searching begins , int key - the ISBN we want to find
{
    int i = 0; // represents the index of the key we are currently checking

    while (i < node->keys.size() && key > node->keys[i])
    {
        i++;
    }
    if (i < node->keys.size() && key == node->keys[i])
    {
        cout << "Key Found";
        return;
    }

    if (node->leaf)
    {
        cout << "Key Not Found";
        return;
    }

    search(node->children[i], key);
}
