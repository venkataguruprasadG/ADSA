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

int main()
{
    Node *root = new Node(10);
    root->left = new Node(5);
    root->right = new Node(20);
}