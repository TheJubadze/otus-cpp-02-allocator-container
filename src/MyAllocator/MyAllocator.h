#pragma once

#include "Core.h"

template<typename T>
class API MyAllocator {
public:
    T *allocate(size_t count) const {
        return ::operator new(count * sizeof(T));
    }

    void deallocate(T *ptr, size_t) {
        ::operator delete(ptr);
    }

    template<typename... TArgs>
    void construct(T *ptr, const TArgs &... args) {
        new(ptr) T(args...);
    }

    void destroy(T *ptr) {
        ptr->~T();
    }
};

