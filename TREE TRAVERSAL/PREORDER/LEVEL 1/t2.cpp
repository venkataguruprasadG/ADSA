#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;

    Node(int val)
    {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

void preorder(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    cout << node->data << " ";
    preorder(node->left);
    preorder(node->right);
}

int main()
{
    Node *root = new Node(10);
    root->left = new Node(20);
    root->right = new Node(30);

    root->left->left = new Node(40);

    preorder(root);
    return 0;
}