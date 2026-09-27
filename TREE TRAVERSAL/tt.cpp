#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;

    Node(int value)
    {
        data = value;
        left = nullptr;
        right = nullptr;
    }
};

void inorderTraversal(Node *node)
{
    if (node == nullptr)
    {
        return;
    }
    inorderTraversal(node->left);
    cout << node->data << " ";
    inorderTraversal(node->right);
}

void postorderTraversal(Node *node)
{
    if (node == nullptr)
    {
        return;
    }
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    cout << node->data << " ";
}

void preorderTraversal(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    cout << node->data << " ";
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}

int main()
{
    Node *root = new Node(10);
    root->left = new Node(5);
    root->right = new Node(20);

    root->left->left = new Node(3);
    root->left->right = new Node(7);
    root->right->right = new Node(25);
    root->right->left = new Node(15);

    cout << "Preorder Traversal: " << endl;
    preorderTraversal(root);

    cout << "Inorder Traversal: " << endl;
    inorderTraversal(root);
}
