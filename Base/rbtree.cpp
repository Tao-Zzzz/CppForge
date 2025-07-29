#include <iostream>

enum Color
{
    RED,
    BLACK
};

template <typename Key, typename Value>
struct RBTreeNode
{
    Key key;
    Value value;
    Color color;
    RBTreeNode *parent;
    RBTreeNode *left;
    RBTreeNode *right;

    RBTreeNode(Key k, Value v)
        : key(k), value(v), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename Key, typename Value>
class RedBlackTree
{
private:
    RBTreeNode<Key, Value> *root;

    void leftRotate(RBTreeNode<Key, Value> *x)
    {
        RBTreeNode<Key, Value> *y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBTreeNode<Key, Value> *y)
    {
        RBTreeNode<Key, Value> *x = y->left;
        y->left = x->right;
        if (x->right != nullptr)
            x->right->parent = y;

        x->parent = y->parent;
        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;

        x->right = y;
        y->parent = x;
    }

    void insertFixUp(RBTreeNode<Key, Value> *z)
    {
        while (z->parent != nullptr && z->parent->color == RED)
        {
            if (z->parent == z->parent->parent->left)
            {
                RBTreeNode<Key, Value> *y = z->parent->parent->right; // 叔叔节点
                if (y != nullptr && y->color == RED)
                {
                    // 情况1
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        // 情况2
                        z = z->parent;
                        leftRotate(z);
                    }
                    // 情况3
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                // 父节点是右子节点，情况对称
                RBTreeNode<Key, Value> *y = z->parent->parent->left; // 叔叔节点
                if (y != nullptr && y->color == RED)
                {
                    // 情况1
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        // 情况2
                        z = z->parent;
                        rightRotate(z);
                    }
                    // 情况3
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void inorderHelper(RBTreeNode<Key, Value> *node) const
    {
        if (node == nullptr)
            return;
        inorderHelper(node->left);
        std::cout << node->key << " ";
        inorderHelper(node->right);
    }

public:
    RedBlackTree() : root(nullptr) {}

    RBTreeNode<Key, Value> *getRoot() const { return root; }

    void insert(const Key &key, const Value &value)
    {
        RBTreeNode<Key, Value> *z = new RBTreeNode<Key, Value>(key, value);
        RBTreeNode<Key, Value> *y = nullptr;
        RBTreeNode<Key, Value> *x = root;

        while (x != nullptr)
        {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        // 插入后修正红黑树性质
        insertFixUp(z);
    }

    void inorderTraversal() const
    {
        inorderHelper(root);
        std::cout << std::endl;
    }

    // 为简化示例，删除操作未实现
    // 完整实现需要包含 deleteFixUp 等步骤
};