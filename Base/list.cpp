#include <iostream>


// 节点结构体
template <typename T>
struct Node
{
    T data;
    Node *prev;
    Node *next;

    Node(const T &value = T()) : data(value), prev(nullptr), next(nullptr) {}
};

// 迭代器类
template <typename T>
class Iterator
{
public:
    using self_type = Iterator<T>;
    using value_type = T;
    using reference = T &;
    using pointer = T *;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;

    Iterator(Node<T> *ptr = nullptr) : node_ptr(ptr) {}

    // Dereference operator
    reference operator*() const { return node_ptr->data; }

    // Arrow operator
    pointer operator->() const { return &(node_ptr->data); }

    // Pre-increment
    self_type &operator++()
    {
        if (node_ptr)
            node_ptr = node_ptr->next;
        return *this;
    }

    // Post-increment
    self_type operator++(int)
    {
        self_type temp = *this;
        ++(*this);
        return temp;
    }

    // Pre-decrement
    self_type &operator--()
    {
        if (node_ptr)
            node_ptr = node_ptr->prev;
        return *this;
    }

    // Post-decrement
    self_type operator--(int)
    {
        self_type temp = *this;
        --(*this);
        return temp;
    }

    // Equality comparison
    bool operator==(const self_type &other) const
    {
        return node_ptr == other.node_ptr;
    }

    // Inequality comparison
    bool operator!=(const self_type &other) const
    {
        return node_ptr != other.node_ptr;
    }

private:
    Node<T> *node_ptr;

    friend class List<T>;
};

// List 类
template <typename T>
class List
{
public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<T>;

    // 构造函数
    List()
    {
        head = new Node<T>(); // 头哨兵
        tail = new Node<T>(); // 尾哨兵
        head->next = tail;
        tail->prev = head;
    }

    // 析构函数
    ~List()
    {
        clear();
        delete head;
        delete tail;
    }

    // 禁止拷贝构造和赋值操作（简化实现）
    List(const List &other) = delete;
    List &operator=(const List &other) = delete;

    // 插入元素到迭代器位置之前
    iterator insert(iterator pos, const T &value)
    {
        Node<T> *current = pos.node_ptr;
        Node<T> *new_node = new Node<T>(value);

        Node<T> *prev_node = current->prev;

        new_node->next = current;
        new_node->prev = prev_node;

        prev_node->next = new_node;
        current->prev = new_node;

        return iterator(new_node);
    }

    // 删除迭代器指向的元素
    iterator erase(iterator pos)
    {
        Node<T> *current = pos.node_ptr;
        if (current == head || current == tail)
        {
            // 不能删除哨兵节点
            return pos;
        }

        Node<T> *prev_node = current->prev;
        Node<T> *next_node = current->next;

        prev_node->next = next_node;
        next_node->prev = prev_node;

        delete current;

        return iterator(next_node);
    }

    // 在头部插入元素
    void push_front(const T &value)
    {
        insert(begin(), value);
    }

    // 在尾部插入元素
    void push_back(const T &value)
    {
        insert(end(), value);
    }

    // 在头部删除元素
    void pop_front()
    {
        if (!empty())
        {
            erase(begin());
        }
    }

    // 在尾部删除元素
    void pop_back()
    {
        if (!empty())
        {
            iterator temp = end();
            --temp;
            erase(temp);
        }
    }

    // 获取头元素引用
    T &front()
    {
        return head->next->data;
    }

    // 获取尾元素引用
    T &back()
    {
        return tail->prev->data;
    }

    // 判断是否为空
    bool empty() const
    {
        return head->next == tail;
    }

    // 获取链表大小（O(n)复杂度）
    size_t size() const
    {
        size_t count = 0;
        for (auto it = begin(); it != end(); ++it)
        {
            ++count;
        }
        return count;
    }

    // 清空链表
    void clear()
    {
        Node<T> *current = head->next;
        while (current != tail)
        {
            Node<T> *temp = current;
            current = current->next;
            delete temp;
        }
        head->next = tail;
        tail->prev = head;
    }

    // 获取开始迭代器
    iterator begin()
    {
        return iterator(head->next);
    }

    // 获取结束迭代器
    iterator end()
    {
        return iterator(tail);
    }

    // 打印链表（辅助函数）
    void print() const
    {
        Node<T> *current = head->next;
        while (current != tail)
        {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

private:
    Node<T> *head; // 头哨兵
    Node<T> *tail; // 尾哨兵
};

// 测试代码
int main()
{
    List<int> lst;

    // 插入元素
    lst.push_back(10);            // 链表: 10
    lst.push_front(5);            // 链表: 5, 10
    lst.push_back(15);            // 链表: 5, 10, 15
    lst.insert(++lst.begin(), 7); // 链表: 5, 7, 10, 15

    // 打印链表
    std::cout << "链表内容: ";
    lst.print(); // 输出: 5 7 10 15

    // 删除元素
    lst.pop_front(); // 链表: 7, 10, 15
    lst.pop_back();  // 链表: 7, 10

    // 打印链表
    std::cout << "删除头尾后链表内容: ";
    lst.print(); // 输出: 7 10

    // 插入和删除
    auto it = lst.begin();
    lst.insert(it, 3); // 链表: 3, 7, 10
    lst.erase(++it);   // 链表: 3, 10

    // 打印链表
    std::cout << "插入和删除后链表内容: ";
    lst.print(); // 输出: 3 10

    // 清空链表
    lst.clear();
    std::cout << "清空后，链表是否为空: " << (lst.empty() ? "是" : "否") << std::endl;

    return 0;
}