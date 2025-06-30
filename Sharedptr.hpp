#pragma once


namespace forge{
    template<typename T>
    class SharedPtr {
    private:
        T* ptr;
        size_t* count;
    public:
        SharedPtr() : ptr(nullptr), count(new size_t(0)){}
        explicit SharedPtr(T* raw_ptr) : ptr(raw_ptr), count(new size_t(1)){}

        SharedPtr(const SharedPtr& other) : ptr(other.ptr), count(other.count) {
            if (count)
                (*count)++;
        }
        SharedPtr(SharedPtr&& other) :ptr(other.ptr), count(other.count) {
            other.ptr = nullptr;
            other.count = nullptr;
        }

        ~SharedPtr() {
            if (count && --(*count) == 0) {
                delete(ptr);
                delete(count);
            }
        }

        SharedPtr& operator=(const SharedPtr& other) {
            if (this != &other) {
                if (count && --(*count) == 0) {
                    delete ptr;
                    delete count;
                }
                ptr = other.ptr;
                count = other.count;
                if (count)
                    (*count)++;
            }
            return *this;
        }
        SharedPtr& operator=(SharedPtr&& other) {
            if (this != &other) {
                if (count && --(*count) == 0) {
                    delete ptr;
                    delete count;
                }
                ptr = other.ptr;
                count = other.count;
                other.ptr = nullptr;
                other.count = nullptr;
            }
            return *this;
        }

        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }

        T* get() const { return ptr; }
        size_t use_count() { return count ? *count : 0; }

        void reset() {
            if (count && --(*count) == 0) {
                delete(ptr);
                delete(count);
            }
            ptr = nullptr;
            count = new size_t(0);
        }
    };
}