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

    root->left = new Node();
    root->left->data = 5;
    root->left->left = nullptr;
    root->left->right = nullptr;

    root->right = new Node();
    root->right->data = 20;
    root->right->left = nullptr;
    root->right->right = nullptr;

    cout << "Root: " << root->data << endl;
    cout << "Left Child: " << root->left->data << endl;
    cout << "Right Child: " << root->right->data << endl;
}