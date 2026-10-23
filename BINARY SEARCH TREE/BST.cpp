#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;
};

Node *createNode(int val)
{
    Node *newNode = new Node();
    newNode->data = val;
    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

Node *insertNode(Node *root, int val)
{
    if (root == nullptr)
    {
        return createNode(val);
    }

    if (val < root->data)
    {
        root->left = insertNode(root->left, val);
    }
    else
    {
        root->right = insertNode(root->right, val);
    }
    return root;
}

void inorder(Node *root)
{
    if (root == nullptr)
    {
        return;
    }
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}
int main()
{
    Node *root = nullptr;

    root = insertNode(root, 10);
    root = insertNode(root, 5);
    root = insertNode(root, 20);
    root = insertNode(root, 2);
    root = insertNode(root, 7);

    inorder(root);

    return 0;
}