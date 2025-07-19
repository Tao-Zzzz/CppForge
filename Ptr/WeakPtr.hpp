#pragma once
#include <iostream>
#include "Sharedptr.hpp"
namespace forge {
    template<typename T>
    class WeakPtr {
    private:
        T* ptr;
        typename SharedPtr<T>::ControlBlock* control;

    public:
        WeakPtr() : ptr(nullptr), control(nullptr) {}
        WeakPtr(const SharedPtr<T>& sp) : ptr(sp.get()), control(sp.control) {
            if (control) control->weak_count++;
        }
        WeakPtr(const WeakPtr& other) : ptr(other.ptr), control(other.control) {
            if (control) control->weak_count++;
        }
        WeakPtr(WeakPtr&& other) noexcept : ptr(other.ptr), control(other.control) {
            other.ptr = nullptr;
            other.control = nullptr;
        }

        WeakPtr& operator=(const WeakPtr& other) {
            if (this != &other) {
                if (control && --control->weak_count == 0 && control->use_count == 0) {
                    delete control;
                }
                ptr = other.ptr;
                control = other.control;
                if (control) control->weak_count++;
            }
            return *this;
        }
        WeakPtr& operator=(WeakPtr&& other) noexcept {
            if (this != &other) {
                if (control && --control->weak_count == 0 && control->use_count == 0) {
                    delete control;
                }
                ptr = other.ptr;
                control = other.control;
                other.ptr = nullptr;
                other.control = nullptr;
            }
            return *this;
        }

        ~WeakPtr() {
            if (control && --control->weak_count == 0 && control->use_count == 0) {
                delete control;
            }
        }

        bool expired() const {
            //std::cout << "expired: control = " << control << ", use_count = " << (control ? control->use_count : 0) << "\n";
            return !control || control->use_count == 0;
        }

        SharedPtr<T> lock() const {
            if (expired()) {
                return SharedPtr<T>();
            }
            return SharedPtr<T>(ptr, control);
        }
    };
}