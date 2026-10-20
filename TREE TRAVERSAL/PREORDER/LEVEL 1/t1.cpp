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

void PreorderTraversal(Node *node)
{
    if (node == nullptr)
    {
        return;
    }

    cout << node->data << " ";
    PreorderTraversal(node->left);
    PreorderTraversal(node->right);
}