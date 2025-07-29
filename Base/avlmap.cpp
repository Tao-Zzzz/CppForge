#include <iostream>
#include <string>
#include <vector>
#include <algorithm>   // 用于 std::max
#include <functional>  // 用于 std::function

// 模板化的AVL树节点结构
template <typename KeyType, typename ValueType>
struct AVLNode {
    KeyType key;
    ValueType value;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const KeyType& k, const ValueType& val)
        : key(k), value(val), height(1), left(nullptr), right(nullptr) {}
};

// 获取节点高度
template <typename KeyType, typename ValueType>
int getHeight(AVLNode<KeyType, ValueType>* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// 获取平衡因子
template <typename KeyType, typename ValueType>
int getBalance(AVLNode<KeyType, ValueType>* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// 右旋转
template <typename KeyType, typename ValueType>
AVLNode<KeyType, ValueType>* rightRotate(AVLNode<KeyType, ValueType>* y) {
    AVLNode<KeyType, ValueType>* x = y->left;
    AVLNode<KeyType, ValueType>* T2 = x->right;

    // 执行旋转
    x->right = y;
    y->left = T2;

    // 更新高度
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    // 返回新的根
    return x;
}

// 左旋转
template <typename KeyType, typename ValueType>
AVLNode<KeyType, ValueType>* leftRotate(AVLNode<KeyType, ValueType>* x) {
    AVLNode<KeyType, ValueType>* y = x->right;
    AVLNode<KeyType, ValueType>* T2 = y->left;

    // 执行旋转
    y->left = x;
    x->right = T2;

    // 更新高度
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    // 返回新的根
    return y;
}

// 插入节点
template <typename KeyType, typename ValueType>
AVLNode<KeyType, ValueType>* insertNode(AVLNode<KeyType, ValueType>* node, const KeyType& key, const ValueType& value) {
    // 1. 执行标准的BST插入
    if (node == nullptr)
        return new AVLNode<KeyType, ValueType>(key, value);

    if (key < node->key)
        node->left = insertNode(node->left, key, value);
    else if (key > node->key)
        node->right = insertNode(node->right, key, value);
    else {
        // 如果键已经存在，更新其值
        node->value = value;
        return node;
    }

    // 2. 更新节点高度
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // 3. 获取平衡因子
    int balance = getBalance(node);

    // 4. 根据平衡因子进行旋转

    // 左左情况
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // 右右情况
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // 左右情况
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // 右左情况
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// 查找节点
template <typename KeyType, typename ValueType>
ValueType* searchNode(AVLNode<KeyType, ValueType>* node, const KeyType& key) {
    if (node == nullptr)
        return nullptr;

    if (key == node->key)
        return &(node->value);
    else if (key < node->key)
        return searchNode(node->left, key);
    else
        return searchNode(node->right, key);
}

// 获取最小值节点
template <typename KeyType, typename ValueType>
AVLNode<KeyType, ValueType>* getMinValueNode(AVLNode<KeyType, ValueType>* node) {
    AVLNode<KeyType, ValueType>* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// 删除节点
template <typename KeyType, typename ValueType>
AVLNode<KeyType, ValueType>* deleteNode(AVLNode<KeyType, ValueType>* root, const KeyType& key) {
    // 1. 执行标准的BST删除
    if (root == nullptr)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // 节点有一个或没有子节点
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode<KeyType, ValueType>* temp = root->left ? root->left : root->right;

            // 没有子节点
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else // 一个子节点
                *root = *temp; // 复制内容

            delete temp;
        }
        else {
            // 节点有两个子节点，获取中序后继
            AVLNode<KeyType, ValueType>* temp = getMinValueNode(root->right);
            // 复制中序后继的内容到此节点
            root->key = temp->key;
            root->value = temp->value;
            // 删除中序后继
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // 如果树只有一个节点
    if (root == nullptr)
        return root;

    // 2. 更新节点高度
    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    // 3. 获取平衡因子
    int balance = getBalance(root);

    // 4. 根据平衡因子进行旋转

    // 左左情况
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // 左右情况
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // 右右情况
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // 右左情况
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// 模板化的AVLMap类
template <typename KeyType, typename ValueType>
class AVLMap {
private:
    AVLNode<KeyType, ValueType>* root;

    // 中序遍历辅助函数
    void inorderHelper(AVLNode<KeyType, ValueType>* node, std::vector<std::pair<KeyType, ValueType>>& res) const {
        if (node != nullptr) {
            inorderHelper(node->left, res);
            res.emplace_back(node->key, node->value);
            inorderHelper(node->right, res);
        }
    }

public:
    AVLMap() : root(nullptr) {}

    // 插入或更新键值对
    void put(const KeyType& key, const ValueType& value) {
        root = insertNode(root, key, value);
    }

    // 查找值，返回指向值的指针，如果键不存在则返回nullptr
    ValueType* get(const KeyType& key) {
        return searchNode(root, key);
    }

    // 删除键值对
    void remove(const KeyType& key) {
        root = deleteNode(root, key);
    }

    // 中序遍历，返回有序的键值对
    std::vector<std::pair<KeyType, ValueType>> inorderTraversal() const {
        std::vector<std::pair<KeyType, ValueType>> res;
        inorderHelper(root, res);
        return res;
    }

    // 析构函数，释放所有节点的内存
    ~AVLMap() {
        // 使用后序遍历释放节点
        std::function<void(AVLNode<KeyType, ValueType>*)> destroy = [&](AVLNode<KeyType, ValueType>* node) {
            if (node) {
                destroy(node->left);
                destroy(node->right);
                delete node;
            }
        };
        destroy(root);
    }
};

// 示例主函数
int main() {
    // 示例 1：int 键，std::string 值
    std::cout << "示例 1：int 键，std::string 值\n";
    AVLMap<int, std::string> avlMap1;

    // 插入键值对
    avlMap1.put(10, "十");
    avlMap1.put(20, "二十");
    avlMap1.put(30, "三十");
    avlMap1.put(40, "四十");
    avlMap1.put(50, "五十");
    avlMap1.put(25, "二十五");

    // 中序遍历
    std::vector<std::pair<int, std::string>> traversal1 = avlMap1.inorderTraversal();
    std::cout << "中序遍历: ";
    for (const auto& pair : traversal1) {
        std::cout << "(" << pair.first << ", \"" << pair.second << "\") ";
    }
    std::cout << std::endl;

    // 查找键
    std::string* val1 = avlMap1.get(20);
    if (val1)
        std::cout << "获取键20的值: " << *val1 << std::endl;
    else
        std::cout << "键20不存在。" << std::endl;

    val1 = avlMap1.get(25);
    if (val1)
        std::cout << "获取键25的值: " << *val1 << std::endl;
    else
        std::cout << "键25不存在。" << std::endl;

    val1 = avlMap1.get(60);
    if (val1)
        std::cout << "获取键60的值: " << *val1 << std::endl;
    else
        std::cout << "键60不存在。" << std::endl;

    // 删除键20
    avlMap1.remove(20);
    std::cout << "删除键20后，中序遍历: ";
    traversal1 = avlMap1.inorderTraversal();
    for (const auto& pair : traversal1) {
        std::cout << "(" << pair.first << ", \"" << pair.second << "\") ";
    }
    std::cout << std::endl;

    std::cout << "\n-----------------------------\n";

    // 示例 2：std::string 键，double 值
    std::cout << "示例 2：std::string 键，double 值\n";
    AVLMap<std::string, double> avlMap2;

    // 插入键值对
    avlMap2.put("apple", 1.99);
    avlMap2.put("banana", 0.99);
    avlMap2.put("cherry", 2.99);
    avlMap2.put("date", 3.49);
    avlMap2.put("elderberry", 5.99);
    avlMap2.put("fig", 2.49);

    // 中序遍历
    std::vector<std::pair<std::string, double>> traversal2 = avlMap2.inorderTraversal();
    std::cout << "中序遍历: ";
    for (const auto& pair : traversal2) {
        std::cout << "(\"" << pair.first << "\", " << pair.second << ") ";
    }
    std::cout << std::endl;

    // 查找键
    double* val2 = avlMap2.get("banana");
    if (val2)
        std::cout << "获取键\"banana\"的值: " << *val2 << std::endl;
    else
        std::cout << "键\"banana\"不存在。" << std::endl;

    val2 = avlMap2.get("fig");
    if (val2)
        std::cout << "获取键\"fig\"的值: " << *val2 << std::endl;
    else
        std::cout << "键\"fig\"不存在。" << std::endl;

    val2 = avlMap2.get("grape");
    if (val2)
        std::cout << "获取键\"grape\"的值: " << *val2 << std::endl;
    else
        std::cout << "键\"grape\"不存在。" << std::endl;

    // 删除键"banana"
    avlMap2.remove("banana");
    std::cout << "删除键\"banana\"后，中序遍历: ";
    traversal2 = avlMap2.inorderTraversal();
    for (const auto& pair : traversal2) {
        std::cout << "(\"" << pair.first << "\", " << pair.second << ") ";
    }
    std::cout << std::endl;

    return 0;
}