#pragma once

#include "Log.h"

template<typename T>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() = default;

    template<typename U>
    explicit MyAllocator(const MyAllocator<U> &right) {};

    T *allocate(size_t count) const {
        CORE_TRACE("Allocating: {} x {} = {} bytes", count, sizeof(T), count * sizeof(T));
        return reinterpret_cast<T *>(::operator new(count * sizeof(T)));
    }

    void deallocate(T *ptr, size_t n) {
        ::operator delete(ptr);
        CORE_TRACE("{} bytes deallocated", n);
    }

    template<typename... TArgs>
    void construct(T *ptr, const TArgs &... args) {
        CORE_TRACE("Constructing");
        new(ptr) T(args...);
        CORE_TRACE("Constructed");
    }

    void destroy(T *ptr) {
        CORE_TRACE("Destroying");
        ptr->~T();
        CORE_TRACE("Destroyed");
    }
};
