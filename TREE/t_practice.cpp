#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *left;
    Node *right;
};

int main()
{
    Node *root = new Node{50, nullptr, nullptr};

    root->left = new Node{25, nullptr, nullptr};
    root->right = new Node{75, nullptr, nullptr};

    return 0;
}