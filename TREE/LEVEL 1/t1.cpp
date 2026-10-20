#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node *right;
    Node *left;

    Node(int val)
    {
        data = val;
        right = nullptr;
        left = nullptr;
    }
};

int main()
{
    Node *root = new Node(10);
}