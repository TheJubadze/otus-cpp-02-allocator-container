#pragma once

template<typename T>
class MyAllocator {
public:
    using value_type = T;

    MyAllocator() = default;

    template<typename U>
    explicit MyAllocator(const MyAllocator<U> &right) {};

    T *allocate(size_t count) const {
        return reinterpret_cast<T *>(::operator new(count * sizeof(T)));
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
