
#include <iostream>
#include <vector>
using namespace std;

// B-TREE NODE
class BTreeNode
{
public:
    vector<int> keys;
    vector<BTreeNode *> children;
    bool leaf;

    BTreeNode(bool isLeaf)
    {
        leaf = isLeaf;
    }
};

// B-TREE
class BTree
{
public:
    BTreeNode *root;
    int t;

    BTree()
    {
        root = new BTreeNode(true);
        t = 2;
    }

    // SEARCH
    bool search(BTreeNode *node, int key)
    {
        int i = 0;

        while (i < node->keys.size() && key > node->keys[i])
        {
            i++;
        }

        if (i < node->keys.size() && key == node->keys[i])
        {
            return true;
        }

        if (node->leaf)
        {
            return false;
        }

        return search(node->children[i], key);
    }

    // INSERT
    void insert(int key)
    {
        // Avoid duplicate keys
        if (search(root, key))
        {
            cout << key << " already exists\n";
            return;
        }

        // If root is full, split it
        if (root->keys.size() == 2 * t - 1)
        {
            BTreeNode *newRoot = new BTreeNode(false);

            newRoot->children.push_back(root);

            splitChild(newRoot, 0);

            root = newRoot;
        }

        insertNonFull(root, key);
    }

    // INSERT INTO NON-FULL NODE
    void insertNonFull(BTreeNode *node, int key)
    {
        int i = node->keys.size() - 1;

        if (node->leaf)
        {
            node->keys.push_back(key);

            while (i >= 0 && node->keys[i] > key)
            {
                node->keys[i + 1] = node->keys[i];
                i--;
            }

            node->keys[i + 1] = key;
        }
        else
        {
            while (i >= 0 && node->keys[i] > key)
            {
                i--;
            }

            i++;

            // Split full child before descending
            if (node->children[i]->keys.size() == 2 * t - 1)
            {
                splitChild(node, i);

                if (key > node->keys[i])
                {
                    i++;
                }
            }

            insertNonFull(node->children[i], key);
        }
    }

    // SPLIT CHILD
    void splitChild(BTreeNode *parent, int i)
    {
        BTreeNode *fullChild = parent->children[i];

        int middleKey = fullChild->keys[t - 1];

        BTreeNode *newChild = new BTreeNode(fullChild->leaf);

        // Move keys to new sibling
        for (int j = t; j < 2 * t - 1; j++)
        {
            newChild->keys.push_back(fullChild->keys[j]);
        }

        // Keep left-side keys in original child
        fullChild->keys.resize(t - 1);

        // Move child pointers if not a leaf
        if (!fullChild->leaf)
        {
            for (int j = t; j < 2 * t; j++)
            {
                newChild->children.push_back(fullChild->children[j]);
            }

            fullChild->children.resize(t);
        }

        // Insert new sibling into parent
        parent->children.insert(
            parent->children.begin() + i + 1,
            newChild);

        // Promote middle key to parent
        parent->keys.insert(
            parent->keys.begin() + i,
            middleKey);
    }

    // TRAVERSAL
    void traverse(BTreeNode *node)
    {
        int i;

        for (i = 0; i < node->keys.size(); i++)
        {
            if (!node->leaf)
            {
                traverse(node->children[i]);
            }

            cout << node->keys[i] << " ";
        }

        if (!node->leaf)
        {
            traverse(node->children[i]);
        }
    }

    // FIND FIRST KEY GREATER THAN OR EQUAL TO key
    int findKey(BTreeNode *node, int key)
    {
        int idx = 0;

        while (idx < node->keys.size() && node->keys[idx] < key)
        {
            idx++;
        }

        return idx;
    }

    // GET PREDECESSOR
    int getPredecessor(BTreeNode *node, int idx)
    {
        BTreeNode *current = node->children[idx];

        while (!current->leaf)
        {
            current = current->children.back();
        }

        return current->keys.back();
    }

    // GET SUCCESSOR
    int getSuccessor(BTreeNode *node, int idx)
    {
        BTreeNode *current = node->children[idx + 1];

        while (!current->leaf)
        {
            current = current->children[0];
        }

        return current->keys[0];
    }

    // REMOVE FROM LEAF
    void removeFromLeaf(BTreeNode *node, int idx)
    {
        node->keys.erase(node->keys.begin() + idx);
    }

    // REMOVE FROM NON-LEAF
    void removeFromNonLeaf(BTreeNode *node, int idx)
    {
        int key = node->keys[idx];

        // Case 1: Left child has enough keys
        if (node->children[idx]->keys.size() >= t)
        {
            int pred = getPredecessor(node, idx);

            node->keys[idx] = pred;

            removeKey(node->children[idx], pred);
        }

        // Case 2: Right child has enough keys
        else if (node->children[idx + 1]->keys.size() >= t)
        {
            int succ = getSuccessor(node, idx);

            node->keys[idx] = succ;

            removeKey(node->children[idx + 1], succ);
        }

        // Case 3: Both children have minimum keys
        else
        {
            merge(node, idx);

            removeKey(node->children[idx], key);
        }
    }

    // BORROW FROM PREVIOUS SIBLING
    void borrowFromPrevious(BTreeNode *node, int idx)
    {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx - 1];

        // Move parent key to child
        child->keys.insert(
            child->keys.begin(),
            node->keys[idx - 1]);

        // Move sibling's last child pointer
        if (!child->leaf)
        {
            child->children.insert(
                child->children.begin(),
                sibling->children.back());

            sibling->children.pop_back();
        }

        // Move sibling's last key to parent
        node->keys[idx - 1] = sibling->keys.back();

        sibling->keys.pop_back();
    }

    // BORROW FROM NEXT SIBLING
    void borrowFromNext(BTreeNode *node, int idx)
    {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx + 1];

        // Move parent key to child
        child->keys.push_back(node->keys[idx]);

        // Move sibling's first child pointer
        if (!child->leaf)
        {
            child->children.push_back(sibling->children[0]);

            sibling->children.erase(sibling->children.begin());
        }

        // Move sibling's first key to parent
        node->keys[idx] = sibling->keys[0];

        sibling->keys.erase(sibling->keys.begin());
    }

    // MERGE TWO CHILDREN
    void merge(BTreeNode *node, int idx)
    {
        BTreeNode *child = node->children[idx];
        BTreeNode *sibling = node->children[idx + 1];

        // Bring parent key down
        child->keys.push_back(node->keys[idx]);

        // Copy sibling keys
        for (int i = 0; i < sibling->keys.size(); i++)
        {
            child->keys.push_back(sibling->keys[i]);
        }

        // Copy sibling child pointers
        if (!child->leaf)
        {
            for (int i = 0; i < sibling->children.size(); i++)
            {
                child->children.push_back(sibling->children[i]);
            }
        }

        // Remove key from parent
        node->keys.erase(node->keys.begin() + idx);

        // Remove sibling pointer from parent
        node->children.erase(node->children.begin() + idx + 1);

        delete sibling;
    }

    // FILL A CHILD THAT HAS MINIMUM KEYS
    void fill(BTreeNode *node, int idx)
    {
        // Borrow from previous sibling
        if (idx != 0 &&
            node->children[idx - 1]->keys.size() >= t)
        {
            borrowFromPrevious(node, idx);
        }

        // Borrow from next sibling
        else if (idx != node->keys.size() &&
                 node->children[idx + 1]->keys.size() >= t)
        {
            borrowFromNext(node, idx);
        }

        // Merge with next sibling
        else
        {
            if (idx != node->keys.size())
            {
                merge(node, idx);
            }

            // Merge with previous sibling
            else
            {
                merge(node, idx - 1);
            }
        }
    }

    // MAIN DELETION FUNCTION
    void removeKey(BTreeNode *node, int key)
    {
        int idx = findKey(node, key);

        // Key is present in this node
        if (idx < node->keys.size() &&
            node->keys[idx] == key)
        {
            if (node->leaf)
            {
                removeFromLeaf(node, idx);
            }
            else
            {
                removeFromNonLeaf(node, idx);
            }
        }

        // Key is not present in this node
        else
        {
            if (node->leaf)
            {
                return;
            }

            bool flag = (idx == node->keys.size());

            // Ensure child has enough keys before descending
            if (node->children[idx]->keys.size() < t)
            {
                fill(node, idx);
            }

            // Child may have merged with previous sibling
            if (flag && idx > node->keys.size())
            {
                removeKey(node->children[idx - 1], key);
            }
            else
            {
                removeKey(node->children[idx], key);
            }
        }
    }

    // PUBLIC DELETE FUNCTION
    void remove(int key)
    {
        if (!search(root, key))
        {
            cout << key << " not found. Cannot delete.\n";
            return;
        }

        removeKey(root, key);

        // If root becomes empty
        if (root->keys.empty())
        {
            BTreeNode *oldRoot = root;

            if (root->leaf)
            {
                root = new BTreeNode(true);
            }
            else
            {
                root = root->children[0];
            }

            delete oldRoot;
        }
    }

    // DISPLAY TREE IN SORTED ORDER
    void display()
    {
        if (root->keys.empty())
        {
            cout << "Tree is empty\n";
            return;
        }

        traverse(root);
        cout << endl;
    }

    // CLEAN UP ALL NODES
    void destroyTree(BTreeNode *node)
    {
        if (node == nullptr)
        {
            return;
        }

        for (int i = 0; i < node->children.size(); i++)
        {
            destroyTree(node->children[i]);
        }

        delete node;
    }

    ~BTree()
    {
        destroyTree(root);
    }
};

// MAIN FUNCTION
int main()
{
    BTree tree;

    int values[] = {
        120, 80, 200, 50, 95, 150, 230,
        30, 60, 90, 110, 170, 250, 140};

    cout << "INSERTION:\n";

    for (int i = 0; i < 14; i++)
    {
        tree.insert(values[i]);
    }

    cout << "B-Tree after insertion:\n";
    tree.display();

    cout << "\nDELETION:\n";

    int deleteValues[] = {50, 200, 120};

    for (int i = 0; i < 3; i++)
    {
        cout << "Deleting " << deleteValues[i] << endl;
        tree.remove(deleteValues[i]);
        tree.display();
    }

    cout << "\nSEARCH:\n";

    int searchKey = 110;

    if (tree.search(tree.root, searchKey))
    {
        cout << searchKey << " found in the B-Tree\n";
    }
    else
    {
        cout << searchKey << " not found in the B-Tree\n";
    }

    return 0;
}
