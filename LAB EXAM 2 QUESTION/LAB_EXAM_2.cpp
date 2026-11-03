#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class BTreeNode
{
public:
    vector<int> keys;
    vector<BTreeNode *> child;
    bool leaf;

    BTreeNode(bool isLeaf) : leaf(isLeaf) {}
};

class BTree
{
    BTreeNode *root;
    const int t = 2; // Order 4: minimum degree 2, maximum 3 keys

    void splitChild(BTreeNode *parent, int i)
    {
        BTreeNode *y = parent->child[i];
        BTreeNode *z = new BTreeNode(y->leaf);
        int middle = y->keys[1];

        z->keys.push_back(y->keys[2]);
        y->keys.resize(1);

        if (!y->leaf)
        {
            z->child.push_back(y->child[2]);
            z->child.push_back(y->child[3]);
            y->child.resize(2);
        }

        parent->child.insert(parent->child.begin() + i + 1, z);
        parent->keys.insert(parent->keys.begin() + i, middle);
    }

    bool insertBottomUp(BTreeNode *node, int key)
    {
        int i = 0;
        while (i < (int)node->keys.size() && key > node->keys[i])
            ++i;

        if (node->leaf)
        {
            node->keys.insert(node->keys.begin() + i, key);
        }
        else
        {
            if (insertBottomUp(node->child[i], key))
            {
                BTreeNode *full = node->child[i];
                BTreeNode *right = new BTreeNode(full->leaf);
                int middle = full->keys[1];

                right->keys.assign(full->keys.begin() + 2, full->keys.end());
                full->keys.resize(1);

                if (!full->leaf)
                {
                    right->child.assign(full->child.begin() + 2, full->child.end());
                    full->child.resize(2);
                }

                node->keys.insert(node->keys.begin() + i, middle);
                node->child.insert(node->child.begin() + i + 1, right);
            }
        }
        return node->keys.size() == 4;
    }

    int findKey(BTreeNode *node, int key)
    {
        int i = 0;
        while (i < (int)node->keys.size() && node->keys[i] < key)
            ++i;
        return i;
    }

    void borrowPrev(BTreeNode *node, int i)
    {
        BTreeNode *c = node->child[i];
        BTreeNode *s = node->child[i - 1];

        c->keys.insert(c->keys.begin(), node->keys[i - 1]);
        node->keys[i - 1] = s->keys.back();
        s->keys.pop_back();

        if (!c->leaf)
        {
            c->child.insert(c->child.begin(), s->child.back());
            s->child.pop_back();
        }
    }

    void borrowNext(BTreeNode *node, int i)
    {
        BTreeNode *c = node->child[i];
        BTreeNode *s = node->child[i + 1];

        c->keys.push_back(node->keys[i]);
        node->keys[i] = s->keys.front();
        s->keys.erase(s->keys.begin());

        if (!c->leaf)
        {
            c->child.push_back(s->child.front());
            s->child.erase(s->child.begin());
        }
    }

    void merge(BTreeNode *node, int i)
    {
        BTreeNode *c = node->child[i];
        BTreeNode *s = node->child[i + 1];

        c->keys.push_back(node->keys[i]);
        for (int k : s->keys)
            c->keys.push_back(k);
        if (!c->leaf)
            for (BTreeNode *p : s->child)
                c->child.push_back(p);

        node->keys.erase(node->keys.begin() + i);
        node->child.erase(node->child.begin() + i + 1);
        delete s;
    }

    void fill(BTreeNode *node, int i)
    {
        if (i > 0 && node->child[i - 1]->keys.size() >= 2)
            borrowPrev(node, i);
        else if (i < (int)node->keys.size() &&
                 node->child[i + 1]->keys.size() >= 2)
            borrowNext(node, i);
        else if (i < (int)node->keys.size())
            merge(node, i);
        else
            merge(node, i - 1);
    }

    int predecessor(BTreeNode *node, int i)
    {
        node = node->child[i];
        while (!node->leaf)
            node = node->child.back();
        return node->keys.back();
    }

    int successor(BTreeNode *node, int i)
    {
        node = node->child[i + 1];
        while (!node->leaf)
            node = node->child.front();
        return node->keys.front();
    }

    void removeFromNonLeaf(BTreeNode *node, int i)
    {
        int key = node->keys[i];

        if (node->child[i]->keys.size() >= 2)
        {
            int pred = predecessor(node, i);
            node->keys[i] = pred;
            removeKey(node->child[i], pred);
        }
        else if (node->child[i + 1]->keys.size() >= 2)
        {
            int succ = successor(node, i);
            node->keys[i] = succ;
            removeKey(node->child[i + 1], succ);
        }
        else
        {
            merge(node, i);
            removeKey(node->child[i], key);
        }
    }

    void removeKey(BTreeNode *node, int key)
    {
        int i = findKey(node, key);

        if (i < (int)node->keys.size() && node->keys[i] == key)
        {
            if (node->leaf)
                node->keys.erase(node->keys.begin() + i);
            else
                removeFromNonLeaf(node, i);
            return;
        }

        if (node->leaf)
            return;

        bool wasLastChild = (i == (int)node->keys.size());
        if (node->child[i]->keys.size() < 2)
            fill(node, i);

        if (wasLastChild && i > (int)node->keys.size())
            removeKey(node->child[i - 1], key);
        else
            removeKey(node->child[i], key);
    }

    void destroy(BTreeNode *node)
    {
        if (!node)
            return;
        for (BTreeNode *p : node->child)
            destroy(p);
        delete node;
    }

    void printLevels(BTreeNode *start)
    {
        if (!start || start->keys.empty())
        {
            cout << "Tree is empty\n";
            return;
        }

        queue<BTreeNode *> q;
        q.push(start);
        int level = 0;

        while (!q.empty())
        {
            int count = (int)q.size();
            cout << "Level " << level++ << ": ";
            while (count--)
            {
                BTreeNode *p = q.front();
                q.pop();

                cout << "[";
                for (int i = 0; i < (int)p->keys.size(); ++i)
                {
                    if (i)
                        cout << " ";
                    cout << p->keys[i];
                }
                cout << "] ";

                for (BTreeNode *c : p->child)
                    q.push(c);
            }
            cout << '\n';
        }
    }

public:
    BTree() { root = new BTreeNode(true); }
    ~BTree() { destroy(root); }

    bool search(int key, bool showPath = false)
    {
        BTreeNode *p = root;
        if (showPath)
            cout << "Nodes visited: ";

        while (p)
        {
            if (showPath)
            {
                cout << "[";
                for (int j = 0; j < (int)p->keys.size(); ++j)
                {
                    if (j)
                        cout << " ";
                    cout << p->keys[j];
                }
                cout << "] ";
            }

            int i = findKey(p, key);
            if (i < (int)p->keys.size() && p->keys[i] == key)
            {
                if (showPath)
                    cout << '\n';
                return true;
            }
            if (p->leaf)
            {
                if (showPath)
                    cout << '\n';
                return false;
            }
            p = p->child[i];
        }
        if (showPath)
            cout << '\n';
        return false;
    }

    void insert(int key)
    {
        if (search(key))
        {
            cout << key << " already exists; duplicate ignored.\n";
            return;
        }

        if (insertBottomUp(root, key))
        {
            BTreeNode *oldRoot = root;
            BTreeNode *newRoot = new BTreeNode(false);
            BTreeNode *right = new BTreeNode(oldRoot->leaf);

            int middle = oldRoot->keys[1];
            right->keys.assign(oldRoot->keys.begin() + 2, oldRoot->keys.end());
            oldRoot->keys.resize(1);

            if (!oldRoot->leaf)
            {
                right->child.assign(oldRoot->child.begin() + 2, oldRoot->child.end());
                oldRoot->child.resize(2);
            }

            newRoot->keys.push_back(middle);
            newRoot->child.push_back(oldRoot);
            newRoot->child.push_back(right);
            root = newRoot;
        }
    }

    void remove(int key)
    {
        if (!search(key))
        {
            cout << key << " not found; cannot delete.\n";
            return;
        }

        removeKey(root, key);
        if (root->keys.empty())
        {
            BTreeNode *oldRoot = root;
            root = root->leaf ? new BTreeNode(true) : root->child[0];
            delete oldRoot;
        }
    }

    void display() { printLevels(root); }
    bool empty() const { return root->keys.empty(); }
};

int main()
{

    cout << "TEST CASE 1\n";

    BTree test1;
    int a[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    for (int x : a)
        test1.insert(x);

    test1.display();

    BTree tree;
    int values[] = {120, 80, 200, 50, 95, 150, 230,
                    30, 60, 90, 110, 170, 250, 140};

    cout << "INSERTION\n";
    for (int key : values)
        tree.insert(key);
    cout << "B-Tree after insertion:\n";
    tree.display();

    cout << "\nDELETION\n";
    int deleted[] = {50, 200, 120};
    for (int key : deleted)
    {
        cout << "Delete " << key << ":\n";
        tree.remove(key);
        tree.display();
    }

    cout << "\nSEARCH\n";
    int searchKey = 110;
    if (tree.search(searchKey, true))
        cout << searchKey << " found.\n";
    else
        cout << searchKey << " not found.\n";

    cout << "\nEDGE-CASE TESTS\n";

    BTree test;
    cout << "1. Search in empty tree: "
         << (test.search(10) ? "Found" : "Not found") << '\n';

    test.insert(10);
    test.insert(10);
    cout << "2. Duplicate insertion handled.\n";

    test.remove(99);
    cout << "3. Deletion of missing key handled.\n";

    cout << "4. Search for missing key: "
         << (test.search(99) ? "Found" : "Not found") << '\n';

    test.remove(10);
    cout << "5. Delete last key; ";
    test.display();

    return 0;
}
