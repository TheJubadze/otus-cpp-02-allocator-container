#pragma once

#include <cstdlib>
#include <memory>
#include "Log.h"

template<typename T, typename TAllocator = std::allocator<T>>
class MyContainer {
private:
    T *arr;
    size_t size{};
    size_t capacity;
    TAllocator alloc;

    using AllocatorTraits = std::allocator_traits<TAllocator>;

    void DeleteArr();

public:
    explicit MyContainer(TAllocator allocator = TAllocator());
    explicit MyContainer(size_t, const T &value = T(), TAllocator allocator = TAllocator());
    virtual ~MyContainer();

    void reserve(size_t n);
    void resize(size_t n, const T &value = T());
    void push_back(const T &value);

    T &at(size_t index);
    T &operator[](size_t index);
    const T &operator[](size_t index) const;
};

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::MyContainer(TAllocator allocator)
        : alloc(allocator), capacity(1) {
    arr = AllocatorTraits::allocate(alloc, sizeof(T));
}

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::MyContainer(size_t n, const T &value, TAllocator allocator)
        : MyContainer(allocator) {
    size = n;
    arr[size - 1] = value;
}

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::~MyContainer() {
    DeleteArr();
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::reserve(size_t n) {
    TRACE("Reserving {}. Capacity = {}", n, capacity);
    if (n <= capacity) {
        return;
    }

    TRACE("Allocating: {} x {} = {} bytes", n, sizeof(T), n * sizeof(T));
    T *newArr = AllocatorTraits::allocate(alloc, n * sizeof(T));
    TRACE("{} bytes allocated", n * sizeof(T));

    try {
        TRACE("uninitialized_copy starting");
        std::uninitialized_copy(arr, arr + size, newArr);
        TRACE("uninitialized_copy success");
    } catch (...) {
        ERROR("uninitialized_copy failed");
        TRACE(" Deallocating {} elements...", size);
        AllocatorTraits::deallocate(alloc, newArr, size);
        TRACE("{} elements deallocated", size);
        throw;
    }

    DeleteArr();

    arr = newArr;
    capacity = n;
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::resize(size_t n, const T &value) {
    TRACE("Resizing {}. Capacity = {}", n, capacity);
    if (n > capacity) {
        reserve(n);
    }

    for (size_t i = size; i < n; ++i) {
        TRACE("resize: Constructing {}", i);
        AllocatorTraits::construct(alloc, arr + i, value);
        TRACE("resize: {} constructed", i);
    }

    if (n < size) {
        size = n;
    }
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::push_back(const T &value) {
    TRACE("push_back {}. Capacity = {}. Size = {}", value, capacity, size);
    if (capacity == size) {
        reserve(2 * size);
    }

    TRACE("push_back: Constructing {}", value);
    AllocatorTraits::construct(alloc, arr + size, value);
    TRACE("push_back: {} constructed", value);
    ++size;
}

template<typename T, typename TAllocator>
T &MyContainer<T, TAllocator>::at(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Attempt out of range access");
    }
    return arr[index];
}

template<typename T, typename TAllocator>
T &MyContainer<T, TAllocator>::operator[](size_t index) {
    return arr[index];
}

template<typename T, typename TAllocator>
const T &MyContainer<T, TAllocator>::operator[](size_t index) const {
    return arr[index];
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::DeleteArr() {
    TRACE("DeleteArr: {}", size);
    for (size_t i = 0; i < this->size; ++i) {
        TRACE("Destroying {}", i);
        AllocatorTraits::destroy(alloc, arr + i);
        TRACE("{} destroyed", i);
    }

    TRACE("Deallocating: {}", size);
    AllocatorTraits::deallocate(alloc, arr, size);
    TRACE("{} deallocated", size);
}
