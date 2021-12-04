#pragma once

#include "Log.h"

namespace OtusAllocator {

    template<typename T>
    class MyAllocator {
    public:
        using value_type = T;

        MyAllocator();

        template<typename U>
        explicit MyAllocator(const MyAllocator<U> &right);

        T *allocate(size_t count) const;

        void deallocate(T *ptr, size_t n);

        template<typename... TArgs>
        void construct(T *ptr, const TArgs &... args);

        void destroy(T *ptr);
    };

    template<typename T>
    MyAllocator<T>::MyAllocator() {
    }

    template<typename T>
    template<typename U>
    MyAllocator<T>::MyAllocator(const MyAllocator<U> &right) {};

    template<typename T>
    T *MyAllocator<T>::allocate(size_t count) const {
        MY_CORE_TRACE("Allocating: {} x {} = {} bytes", count, sizeof(T), count * sizeof(T));
        return reinterpret_cast<T *>(::operator new(count * sizeof(T)));
    }

    template<typename T>
    void MyAllocator<T>::deallocate(T *ptr, size_t n) {
        ::operator delete(ptr);
        MY_CORE_TRACE("{} bytes deallocated", n);
    }

    template<typename T>
    template<typename... TArgs>
    void MyAllocator<T>::construct(T *ptr, const TArgs &... args) {
        MY_CORE_TRACE("Constructing");
        new(ptr) T(args...);
        MY_CORE_TRACE("Constructed");
    }

    template<typename T>
    void MyAllocator<T>::destroy(T *ptr) {
        MY_CORE_TRACE("Destroying");
        ptr->~T();
        MY_CORE_TRACE("Destroyed");
    }

}