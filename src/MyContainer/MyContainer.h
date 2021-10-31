#pragma once

#include <cstdlib>
#include <memory>

template<typename T, typename TAllocator = std::allocator<T>>
class MyContainer {
private:
    T *arr;
    size_t size{};
    size_t capacity;
    TAllocator allocator;

    using AllocatorTraits = std::allocator_traits<TAllocator>;

    void DeleteArr() const;

public:
    MyContainer();
    explicit MyContainer(size_t, const T& value = T(), TAllocator alloc = TAllocator());
    virtual ~MyContainer();
    void reserve(size_t n);
    void resize(size_t n, const T &value = T());
    void push_back(const T &value);

    T &at(size_t index);

    T &operator[](size_t index);
    const T &operator[](size_t index) const;
};

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::MyContainer() {
    arr = reinterpret_cast<T *>(malloc(sizeof(T)));
    capacity = 1;
}

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::MyContainer(size_t n, const T& value, const TAllocator alloc) {
    arr = reinterpret_cast<T *>(malloc(sizeof(T)));
    capacity = 1;
}

template<typename T, typename TAllocator>
MyContainer<T, TAllocator>::~MyContainer() {
    DeleteArr();
}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::reserve(size_t n) {
    if (capacity == 0) {
    }

    if (n <= capacity) {
        return;
    }

    T *newarr = reinterpret_cast<T *>(malloc(n * sizeof(T)));

    try {
        std::uninitialized_copy(arr, arr + size, newarr);
    } catch (...) {
        delete[] reinterpret_cast<int8_t *>(newarr);
        throw;
    }

    DeleteArr();

    arr = newarr;
    capacity = n;

}

template<typename T, typename TAllocator>
void MyContainer<T, TAllocator>::resize(size_t n, const T &value) {
    if (n > capacity) {
        reserve(n);
    }

    for (size_t i = size; i < n; ++i) {
        arr[i] = new(arr + i) T(value);
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

    new(arr + size) T(value);
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
void MyContainer<T, TAllocator>::DeleteArr() const {
    for (size_t i = 0; i < this->size; ++i) {
        this->arr[i].~T();
    }

    delete[] reinterpret_cast<int8_t *>(this->arr);
}
