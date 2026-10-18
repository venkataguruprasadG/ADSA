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
    Node *root = new Node();
    root->data = 10;
    root->left = nullptr;
    root->right = nullptr;

    cout << "Root data: " << root->data << endl;

    Node *leftChild = new Node();
    leftChild->data = 5;
    leftChild->left = nullptr;
    leftChild->right = nullptr;

    root->left = leftChild;

    Node *rightChild = new Node();
    rightChild->data = 15;

    rightChild->left = nullptr;
    rightChild->right = nullptr;

    root->right = rightChild;

    return 0;
}