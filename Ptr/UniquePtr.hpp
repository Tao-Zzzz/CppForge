#pragma once

namespace forge {
	template<typename T>
	class UniquePtr {
	private:
		T* ptr;
	public:
		UniquePtr() : ptr(nullptr){}
		UniquePtr(T* raw_ptr): ptr(raw_ptr){}
		~UniquePtr() {
			delete ptr;
		}

		UniquePtr(const UniquePtr& other) = delete;
		UniquePtr& operator=(const UniquePtr& other) = delete;

		UniquePtr(UniquePtr&& other)noexcept : ptr(other.ptr) {
			other.ptr = nullptr;
		}
		UniquePtr& operator=(UniquePtr&& other) noexcept {
			if (this != &other) {
				delete ptr;
				ptr = other.ptr;
				other.ptr = nullptr;
			}
			return *this;
		}

		T& operator*() const { return *ptr; }
		T* operator->() const { return ptr; }

		T* get() const { return ptr; }
		void reset(T* new_ptr = nullptr) {
			delete ptr;
			ptr = new_ptr;
		}
		T* release() {
			T* tmp = ptr;
			ptr = nullptr;
			return tmp;
		}
	};
}