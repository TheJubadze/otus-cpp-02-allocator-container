#pragma once

#include <cstdlib>
#include <memory>

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
        : alloc(allocator) {
    arr = AllocatorTraits::allocate(alloc, sizeof(T));
    capacity = 1;
}

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::MyContainer(size_t n, const T &value, TAllocator allocator)
        : MyContainer(allocator), size(n) {
    arr[size - 1] = value;
}

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::~MyContainer() {
    DeleteArr();
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::reserve(size_t n) {
    if (n <= capacity) {
        return;
    }

    T *newArr = AllocatorTraits::allocate(alloc, n * sizeof(T));

    try {
        std::uninitialized_copy(arr, arr + size, newArr);
    } catch (...) {
        AllocatorTraits::deallocate(alloc, newArr, size);
        throw;
    }

    DeleteArr();

    arr = newArr;
    capacity = n;
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::resize(size_t n, const T &value) {
    if (n > capacity) {
        reserve(n);
    }

    for (size_t i = size; i < n; ++i) {
        AllocatorTraits::construct(alloc, arr + i, value);
    }

    if (n < size) {
        size = n;
    }
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::push_back(const T &value) {
    if (capacity == size) {
        reserve(2 * size);
    }

    AllocatorTraits::construct(alloc, arr + size, value);
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
    for (size_t i = 0; i < this->size; ++i) {
        AllocatorTraits::destroy(alloc, arr + i);
    }

    AllocatorTraits::deallocate(alloc, arr, size);
}
