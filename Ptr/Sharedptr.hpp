#pragma once

// ǰ������ WeakPtr ģ����
namespace forge {
    template<typename T>
    class WeakPtr;
}

namespace forge{
    template<typename T>
    class SharedPtr {
    private:
        T* ptr;
        struct ControlBlock {
            size_t use_count;  // ǿ���ü���
            size_t weak_count; // �����ü���
            ControlBlock(size_t uc = 0) : use_count(uc), weak_count(0) {}
        };
        ControlBlock* control;

    public:
        SharedPtr() : ptr(nullptr), control(new ControlBlock(0)) {}
        explicit SharedPtr(T* raw_ptr) : ptr(raw_ptr), control(new ControlBlock(1)) {}
        explicit SharedPtr(T* raw_ptr, ControlBlock* ctrl) : ptr(raw_ptr), control(ctrl) {
            if (control) control->use_count++;
        }
        SharedPtr(const SharedPtr& other) : ptr(other.ptr), control(other.control) {
            if (control) control->use_count++;
        }
        SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), control(other.control) {
            other.ptr = nullptr;
            other.control = nullptr;
        }

        ~SharedPtr() {
            if (control && --control->use_count == 0) {
                delete ptr;
                ptr = nullptr; // ȷ�� ptr �ÿ�
                if (control->weak_count == 0) {
                    delete control;
                    control = nullptr;
                }
            }
        }

        SharedPtr& operator=(const SharedPtr& other) {
            if (this != &other) {
                if (control && --control->use_count == 0) {
                    delete ptr;
                    if (control->weak_count == 0) {
                        delete control;
                    }
                }
                ptr = other.ptr;
                control = other.control;
                if (control) control->use_count++;
            }
            return *this;
        }
        SharedPtr& operator=(SharedPtr&& other) noexcept {
            if (this != &other) {
                if (control && --control->use_count == 0) {
                    delete ptr;
                    if (control->weak_count == 0) {
                        delete control;
                    }
                }
                ptr = other.ptr;
                control = other.control;
                other.ptr = nullptr;
                other.control = nullptr;
            }
            return *this;
        }

        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }

        friend class WeakPtr<T>;

        T* get() const { return ptr; }
        size_t use_count() const {
            //std::cout << "use_count: control = " << control << "\n";
            return control ? control->use_count : 0;
        }

        void reset() {
            if (control) {
                if (--control->use_count == 0) {
                    delete ptr;
                    ptr = nullptr;
                    if (control->weak_count == 0) {
                        delete control;
                        control = nullptr;
                    }
                }
            }
            ptr = nullptr;
            control = nullptr;
            //std::cout << "reset: control = " << control << "\n";
        }
    };
}