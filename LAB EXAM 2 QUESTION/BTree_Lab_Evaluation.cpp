#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// ORDER 4 B-TREE: minimum degree t = 2
// Maximum keys = 2*t - 1 = 3; maximum children = 2*t = 4.

class BTreeNode
{
public:
    vector<int> keys;
    vector<BTreeNode *> children;
    bool leaf;

    BTreeNode(bool isLeaf) : leaf(isLeaf) {}
};

class BTree
{
private:
    const int t = 2;
    BTreeNode *root;

    bool searchNode(BTreeNode *node, int key, vector<vector<int>> *path)
    {
        if (node == nullptr)
            return false;

        if (path != nullptr)
            path->push_back(node->keys);

        int i = 0;
        while (i < static_cast<int>(node->keys.size()) && key > node->keys[i])
            i++;

        if (i < static_cast<int>(node->keys.size()) && node->keys[i] == key)
            return true;

        if (node->leaf)
            return false;

        return searchNode(node->children[i], key, path);
    }

    // Bottom-up insertion: split a node only after it temporarily gets 4 keys.
    // For this order-4 B-Tree, key at index 1 is promoted. This produces
    // the insertion structure shown in the question paper.
    void insertRecursive(BTreeNode *node, int key,
                         bool &didSplit, int &promotedKey,
                         BTreeNode *&newRight)
    {
        int index = 0;
        while (index < static_cast<int>(node->keys.size()) &&
               node->keys[index] < key)
            index++;

        if (node->leaf)
        {
            node->keys.insert(node->keys.begin() + index, key);
        }
        else
        {
            bool childSplit = false;
            int childPromotedKey = 0;
            BTreeNode *childNewRight = nullptr;

            insertRecursive(node->children[index], key,
                            childSplit, childPromotedKey, childNewRight);

            if (childSplit)
            {
                node->keys.insert(node->keys.begin() + index,
                                  childPromotedKey);
                node->children.insert(node->children.begin() + index + 1,
                                      childNewRight);
            }
        }

        // A node overflows at 4 keys. Promote the lower middle key.
        if (node->keys.size() == 2 * static_cast<size_t>(t))
        {
            promotedKey = node->keys[t - 1];
            newRight = new BTreeNode(node->leaf);

            for (size_t i = t; i < node->keys.size(); i++)
                newRight->keys.push_back(node->keys[i]);

            if (!node->leaf)
            {
                for (size_t i = t; i < node->children.size(); i++)
                    newRight->children.push_back(node->children[i]);
                node->children.resize(t);
            }

            node->keys.resize(t - 1);
            didSplit = true;
            cout << "Split: promoted key " << promotedKey << ".\n";
        }
    }

    int findKey(BTreeNode *node, int key)
    {
        int index = 0;
        while (index < static_cast<int>(node->keys.size()) && node->keys[index] < key)
            index++;
        return index;
    }

    int getPredecessor(BTreeNode *node, int index)
    {
        BTreeNode *current = node->children[index];
        while (!current->leaf)
            current = current->children.back();
        return current->keys.back();
    }

    int getSuccessor(BTreeNode *node, int index)
    {
        BTreeNode *current = node->children[index + 1];
        while (!current->leaf)
            current = current->children.front();
        return current->keys.front();
    }

    void removeFromLeaf(BTreeNode *node, int index)
    {
        node->keys.erase(node->keys.begin() + index);
        cout << "Deletion case: key removed directly from a leaf.\n";
    }

    void removeFromNonLeaf(BTreeNode *node, int index)
    {
        int key = node->keys[index];

        if (node->children[index]->keys.size() >= static_cast<size_t>(t))
        {
            int predecessor = getPredecessor(node, index);
            cout << "Deletion case: replaced internal key " << key
                 << " with predecessor " << predecessor << ".\n";
            node->keys[index] = predecessor;
            removeKey(node->children[index], predecessor);
        }
        else if (node->children[index + 1]->keys.size() >= static_cast<size_t>(t))
        {
            int successor = getSuccessor(node, index);
            cout << "Deletion case: replaced internal key " << key
                 << " with successor " << successor << ".\n";
            node->keys[index] = successor;
            removeKey(node->children[index + 1], successor);
        }
        else
        {
            merge(node, index);
            removeKey(node->children[index], key);
        }
    }

    void borrowFromPrevious(BTreeNode *node, int index)
    {
        BTreeNode *child = node->children[index];
        BTreeNode *sibling = node->children[index - 1];

        child->keys.insert(child->keys.begin(), node->keys[index - 1]);
        if (!child->leaf)
        {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
        node->keys[index - 1] = sibling->keys.back();
        sibling->keys.pop_back();

        cout << "Rebalancing: borrowed from previous (left) sibling.\n";
    }

    void borrowFromNext(BTreeNode *node, int index)
    {
        BTreeNode *child = node->children[index];
        BTreeNode *sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);
        if (!child->leaf)
        {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
        node->keys[index] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());

        cout << "Rebalancing: borrowed from next (right) sibling.\n";
    }

    void merge(BTreeNode *node, int index)
    {
        BTreeNode *child = node->children[index];
        BTreeNode *sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);
        for (int key : sibling->keys)
            child->keys.push_back(key);

        if (!child->leaf)
        {
            for (BTreeNode *ptr : sibling->children)
                child->children.push_back(ptr);
        }

        node->keys.erase(node->keys.begin() + index);
        node->children.erase(node->children.begin() + index + 1);
        delete sibling;

        cout << "Rebalancing: merged two children using the separator key.\n";
    }

    void fill(BTreeNode *node, int index)
    {
        if (index > 0 && node->children[index - 1]->keys.size() >= static_cast<size_t>(t))
            borrowFromPrevious(node, index);
        else if (index < static_cast<int>(node->keys.size()) &&
                 node->children[index + 1]->keys.size() >= static_cast<size_t>(t))
            borrowFromNext(node, index);
        else if (index < static_cast<int>(node->keys.size()))
            merge(node, index);
        else
            merge(node, index - 1);
    }

    void removeKey(BTreeNode *node, int key)
    {
        int index = findKey(node, key);

        if (index < static_cast<int>(node->keys.size()) && node->keys[index] == key)
        {
            if (node->leaf)
                removeFromLeaf(node, index);
            else
                removeFromNonLeaf(node, index);
            return;
        }

        if (node->leaf)
            return;

        bool wasLastChild = (index == static_cast<int>(node->keys.size()));
        if (node->children[index]->keys.size() < static_cast<size_t>(t))
            fill(node, index);

        if (wasLastChild && index > static_cast<int>(node->keys.size()))
            removeKey(node->children[index - 1], key);
        else
            removeKey(node->children[index], key);
    }

    void destroyTree(BTreeNode *node)
    {
        if (node == nullptr)
            return;
        for (BTreeNode *child : node->children)
            destroyTree(child);
        delete node;
    }

public:
    BTree() : root(new BTreeNode(true)) {}

    // Search and print every node examined, from root to leaf.
    bool search(int key, bool showPath = false)
    {
        vector<vector<int>> path;
        bool found = searchNode(root, key, showPath ? &path : nullptr);

        if (showPath)
        {
            cout << "Nodes visited: ";
            for (size_t i = 0; i < path.size(); i++)
            {
                cout << "[";
                for (size_t j = 0; j < path[i].size(); j++)
                {
                    cout << path[i][j];
                    if (j + 1 < path[i].size()) cout << " ";
                }
                cout << "]";
                if (i + 1 < path.size()) cout << " -> ";
            }
            if (path.empty()) cout << "none (empty tree)";
            cout << "\n";
        }
        return found;
    }

    void insert(int key)
    {
        if (search(key))
        {
            cout << "Duplicate ISBN " << key << " rejected.\n";
            return;
        }

        bool didSplit = false;
        int promotedKey = 0;
        BTreeNode *newRight = nullptr;
        insertRecursive(root, key, didSplit, promotedKey, newRight);

        if (didSplit)
        {
            BTreeNode *newRoot = new BTreeNode(false);
            newRoot->keys.push_back(promotedKey);
            newRoot->children.push_back(root);
            newRoot->children.push_back(newRight);
            root = newRoot;
        }
    }

    void remove(int key)
    {
        if (!search(key))
        {
            cout << "ISBN " << key << " not found; no deletion performed.\n";
            return;
        }

        removeKey(root, key);

        // If the root becomes empty, its only child becomes the new root.
        if (root->keys.empty())
        {
            BTreeNode *oldRoot = root;
            if (root->leaf)
                root = new BTreeNode(true);
            else
                root = root->children[0];
            delete oldRoot;
            cout << "Root adjusted after deletion.\n";
        }
    }

    // Print one level per line so the tree hierarchy is visible.
    void display()
    {
        if (root == nullptr || root->keys.empty())
        {
            cout << "Tree is empty.\n";
            return;
        }

        queue<BTreeNode *> q;
        q.push(root);
        int level = 0;
        while (!q.empty())
        {
            int count = static_cast<int>(q.size());
            cout << "Level " << level << ": ";
            while (count--)
            {
                BTreeNode *node = q.front();
                q.pop();
                cout << "[";
                for (size_t i = 0; i < node->keys.size(); i++)
                {
                    cout << node->keys[i];
                    if (i + 1 < node->keys.size()) cout << " ";
                }
                cout << "] ";
                if (!node->leaf)
                    for (BTreeNode *child : node->children)
                        q.push(child);
            }
            cout << "\n";
            level++;
        }
    }

    void displaySorted()
    {
        cout << "Sorted ISBNs: ";
        if (root->keys.empty())
        {
            cout << "(empty)\n";
            return;
        }
        inorder(root);
        cout << "\n";
    }

    void inorder(BTreeNode *node)
    {
        size_t i;
        for (i = 0; i < node->keys.size(); i++)
        {
            if (!node->leaf) inorder(node->children[i]);
            cout << node->keys[i] << " ";
        }
        if (!node->leaf) inorder(node->children[i]);
    }

    ~BTree()
    {
        destroyTree(root);
    }
};

void runEdgeCaseTests()
{
    cout << "\n========== EDGE CASE TESTS ==========" << endl;

    // 3a: Delete from an empty tree.
    cout << "\n3a. Delete 50 from an empty tree\n";
    {
        BTree tree;
        tree.remove(50);
        tree.display();
    }

    // 3b: Duplicate insertion.
    cout << "\n3b. Insert 120, 80, 120\n";
    {
        BTree tree;
        tree.insert(120);
        tree.insert(80);
        tree.insert(120);
        tree.display();
    }

    // 3c: Search for a key that does not exist.
    cout << "\n3c. Search 999\n";
    {
        BTree tree;
        tree.insert(80);
        tree.insert(120);
        cout << (tree.search(999, true) ? "Found\n" : "Not found\n");
    }

    // 3d: Delete all keys, then insert again.
    cout << "\n3d. Insert 10, 20, 30; delete 20, 10, 30; insert 40\n";
    {
        BTree tree;
        tree.insert(10);
        tree.insert(20);
        tree.insert(30);
        tree.remove(20);
        tree.remove(10);
        tree.remove(30);
        tree.display();
        tree.insert(40);
        tree.display();
    }

    // 3e: Delete the only key, then insert another key.
    cout << "\n3e. Insert 10; delete 10; insert 20\n";
    {
        BTree tree;
        tree.insert(10);
        tree.remove(10);
        tree.display();
        tree.insert(20);
        tree.display();
    }
}

int main()
{
    BTree tree;
    int insertValues[] = {120, 80, 200, 50, 95, 150, 230,
                          30, 60, 90, 110, 170, 250, 140};

    cout << "========== REQUIRED LAB TEST CASE ==========" << endl;
    cout << "B-Tree of order 4 (minimum degree t = 2)\n";

    cout << "\nINSERTIONS:\n";
    for (int key : insertValues)
    {
        cout << "\nInsert " << key << ":\n";
        tree.insert(key);
        cout << "Tree now:\n";
        tree.display();
    }

    cout << "\nB-Tree after all insertions:\n";
    tree.display();
    tree.displaySorted();

    cout << "\nDELETIONS:\n";
    int deleteValues[] = {50, 200, 120};
    for (int key : deleteValues)
    {
        cout << "\nDelete " << key << ":\n";
        tree.remove(key);
        tree.display();
    }

    cout << "\nFINAL B-TREE AFTER ALL DELETIONS:\n";
    tree.display();
    tree.displaySorted();

    cout << "\nSEARCH FOR ISBN 110:\n";
    if (tree.search(110, true))
        cout << "ISBN 110 found in the B-Tree.\n";
    else
        cout << "ISBN 110 not found in the B-Tree.\n";

    runEdgeCaseTests();
    return 0;
}
