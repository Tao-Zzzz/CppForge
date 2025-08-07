#include <vector>
#include <list>
#include <utility> // For std::pair
#include <functional> // For std::hash
#include <iterator> // For iterator_traits
#include <stdexcept> // For exceptions
#include <iostream>

// HashNode 结构定义
template <typename Key, typename T>
struct HashNode {
    std::pair<const Key, T> data;
    HashNode* next;

    HashNode(const Key& key, const T& value)
        : data(std::make_pair(key, value)), next(nullptr) {}
};

// MyHashMap 类定义
template <typename Key, typename T, typename Hash = std::hash<Key>>
class MyHashMap {
public:
    // 迭代器类前向声明
    class Iterator;

    // 类型定义
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = size_t;

    // 构造函数及析构函数
    MyHashMap(size_type initial_capacity = 16, double max_load_factor = 0.75);
    ~MyHashMap();

    // 禁止拷贝构造和赋值
    MyHashMap(const MyHashMap&) = delete;
    MyHashMap& operator=(const MyHashMap&) = delete;

    // 基本操作
    void insert(const Key& key, const T& value);
    T* find(const Key& key);
    const T* find(const Key& key) const;
    bool erase(const Key& key);
    size_type size() const;
    bool empty() const;
    void clear();

    // 迭代器操作
    Iterator begin();
    Iterator end();

    // 迭代器类
    class Iterator {
    public:
        // 迭代器别名
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<Key, T>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        // 构造函数
        Iterator(MyHashMap* map, size_type bucket_index, HashNode<Key, T>* node);

        // 解引用操作符
        reference operator*() const;
        pointer operator->() const;

        // 递增操作符
        Iterator& operator++();
        Iterator operator++(int);

        // 比较操作符
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        MyHashMap* map_;
        size_type bucket_index_;
        HashNode<Key, T>* current_node_;

        // 移动到下一个有效节点
        void advance();
    };

private:
    std::vector<HashNode<Key, T>*> buckets_;
    size_type bucket_count_;
    size_type element_count_;
    double max_load_factor_;
    Hash hash_func_;

    // 辅助函数
    void rehash();
};

// 构造函数
template <typename Key, typename T, typename Hash>
MyHashMap<Key, T, Hash>::MyHashMap(size_type initial_capacity, double max_load_factor)
    : bucket_count_(initial_capacity),
      element_count_(0),
      max_load_factor_(max_load_factor),
      hash_func_(Hash()) {
    buckets_.resize(bucket_count_, nullptr);
}

// 析构函数
template <typename Key, typename T, typename Hash>
MyHashMap<Key, T, Hash>::~MyHashMap() {
    clear();
}

// 插入函数
template <typename Key, typename T, typename Hash>
void MyHashMap<Key, T, Hash>::insert(const Key& key, const T& value) {
    size_type hash_value = hash_func_(key);
    size_type index = hash_value % bucket_count_;

    HashNode<Key, T>* node = buckets_[index];
    while (node != nullptr) {
        if (node->data.first == key) {
            node->data.second = value; // 更新值
            return;
        }
        node = node->next;
    }

    // 键不存在，插入新节点到链表头部
    HashNode<Key, T>* new_node = new HashNode<Key, T>(key, value);
    new_node->next = buckets_[index];
    buckets_[index] = new_node;
    ++element_count_;

    // 检查负载因子，可能需要扩容
    double load_factor = static_cast<double>(element_count_) / bucket_count_;
    if (load_factor > max_load_factor_) {
        rehash();
    }
}

// 查找函数（非常量版本）
template <typename Key, typename T, typename Hash>
T* MyHashMap<Key, T, Hash>::find(const Key& key) {
    size_type hash_value = hash_func_(key);
    size_type index = hash_value % bucket_count_;

    HashNode<Key, T>* node = buckets_[index];
    while (node != nullptr) {
        if (node->data.first == key) {
            return &(node->data.second);
        }
        node = node->next;
    }
    return nullptr;
}

// 查找函数（常量版本）
template <typename Key, typename T, typename Hash>
const T* MyHashMap<Key, T, Hash>::find(const Key& key) const {
    size_type hash_value = hash_func_(key);
    size_type index = hash_value % bucket_count_;

    HashNode<Key, T>* node = buckets_[index];
    while (node != nullptr) {
        if (node->data.first == key) {
            return &(node->data.second);
        }
        node = node->next;
    }
    return nullptr;
}

// 删除函数
template <typename Key, typename T, typename Hash>
bool MyHashMap<Key, T, Hash>::erase(const Key& key) {
    size_type hash_value = hash_func_(key);
    size_type index = hash_value % bucket_count_;

    HashNode<Key, T>* node = buckets_[index];
    HashNode<Key, T>* prev = nullptr;

    while (node != nullptr) {
        if (node->data.first == key) {
            if (prev == nullptr) {
                buckets_[index] = node->next;
            } else {
                prev->next = node->next;
            }
            delete node;
            --element_count_;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false; // 未找到键
}

// 清空函数
template <typename Key, typename T, typename Hash>
void MyHashMap<Key, T, Hash>::clear() {
    for (size_type i = 0; i < bucket_count_; ++i) {
        HashNode<Key, T>* node = buckets_[i];
        while (node != nullptr) {
            HashNode<Key, T>* temp = node;
            node = node->next;
            delete temp;
        }
        buckets_[i] = nullptr;
    }
    element_count_ = 0;
}

// 动态扩容函数
template <typename Key, typename T, typename Hash>
void MyHashMap<Key, T, Hash>::rehash() {
    size_type new_bucket_count = bucket_count_ * 2;
    std::vector<HashNode<Key, T>*> new_buckets(new_bucket_count, nullptr);

    // 重新分配所有元素
    for (size_type i = 0; i < bucket_count_; ++i) {
        HashNode<Key, T>* node = buckets_[i];
        while (node != nullptr) {
            HashNode<Key, T>* next_node = node->next;
            size_type new_index = hash_func_(node->data.first) % new_bucket_count;

            // 插入到新桶的头部
            node->next = new_buckets[new_index];
            new_buckets[new_index] = node;

            node = next_node;
        }
    }

    // 替换旧桶
    buckets_ = std::move(new_buckets);
    bucket_count_ = new_bucket_count;
}

// begin() 函数
template <typename Key, typename T, typename Hash>
typename MyHashMap<Key, T, Hash>::Iterator
MyHashMap<Key, T, Hash>::begin() {
    for (size_type i = 0; i < bucket_count_; ++i) {
        if (buckets_[i] != nullptr) {
            return Iterator(this, i, buckets_[i]);
        }
    }
    return end();
}

// end() 函数
template <typename Key, typename T, typename Hash>
typename MyHashMap<Key, T, Hash>::Iterator
MyHashMap<Key, T, Hash>::end() {
    return Iterator(this, bucket_count_, nullptr);
}

// Iterator 构造函数
template <typename Key, typename T, typename Hash>
MyHashMap<Key, T, Hash>::Iterator::Iterator(MyHashMap* map, size_type bucket_index, HashNode<Key, T>* node)
    : map_(map), bucket_index_(bucket_index), current_node_(node) {}

// 解引用操作符
template <typename Key, typename T, typename Hash>
typename MyHashMap<Key, T, Hash>::Iterator::reference
MyHashMap<Key, T, Hash>::Iterator::operator*() const {
    if (current_node_ == nullptr) {
        throw std::out_of_range("Iterator out of range");
    }
    return current_node_->data;
}

// 成员访问操作符
template <typename Key, typename T, typename Hash>
typename MyHashMap<Key, T, Hash>::Iterator::pointer
MyHashMap<Key, T, Hash>::Iterator::operator->() const {
    if (current_node_ == nullptr) {
        throw std::out_of_range("Iterator out of range");
    }
    return &(current_node_->data);
}

// 前置递增操作符
template <typename Key, typename T, typename Hash>
typename MyHashMap<Key, T, Hash>::Iterator&
MyHashMap<Key, T, Hash>::Iterator::operator++() {
    advance();
    return *this;
}

// 后置递增操作符
template <typename Key, typename T, typename Hash>
typename MyHashMap<Key, T, Hash>::Iterator
MyHashMap<Key, T, Hash>::Iterator::operator++(int) {
    Iterator temp = *this;
    advance();
    return temp;
}

// 比较操作符==
template <typename Key, typename T, typename Hash>
bool MyHashMap<Key, T, Hash>::Iterator::operator==(const Iterator& other) const {
    return map_ == other.map_ &&
           bucket_index_ == other.bucket_index_ &&
           current_node_ == other.current_node_;
}

// 比较操作符!=
template <typename Key, typename T, typename Hash>
bool MyHashMap<Key, T, Hash>::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

// advance 函数：移动到下一个有效节点
template <typename Key, typename T, typename Hash>
void MyHashMap<Key, T, Hash>::Iterator::advance() {
   if(current_node_ != nullptr){
         current_node_ = current_node_->next;
    }
   while(current_node_ == nullptr){
        if(bucket_index_ +1  < map_->bucket_count_){
                   ++bucket_index_;
                   current_node_ = map_->buckets_[bucket_index_];
          }else if(bucket_index_ +1  == map_->bucket_count_){
                   ++bucket_index_;
                   break;
           }
       }	
}

int main() {
    MyHashMap<std::string, int> myMap;

    // 插入元素
    myMap.insert("apple", 3);
    myMap.insert("banana", 5);
    myMap.insert("orange", 2);
    myMap.insert("grape", 7);
    myMap.insert("cherry", 4);

    // 使用迭代器遍历元素
    std::cout << "Map contents:\n";
    for(auto it = myMap.begin(); it != myMap.end(); ++it) {
        std::cout << it->first << " => " << it->second << "\n";
    }

    // 查找元素
    std::string keyToFind = "banana";
    int* value = myMap.find(keyToFind);
    if(value != nullptr) {
        std::cout << "\nFound " << keyToFind << " with value: " << *value << "\n";
    } else {
        std::cout << "\n" << keyToFind << " not found.\n";
    }

    // 删除元素
    myMap.erase("apple");
    myMap.erase("cherry");

    // 再次遍历
    std::cout << "\nAfter erasing apple and cherry:\n";
    for(auto it = myMap.begin(); it != myMap.end(); ++it) {
        std::cout << it->first << " => " << it->second << "\n";
    }

    return 0;
}