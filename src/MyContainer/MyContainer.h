#pragma once

#include <cstdlib>
#include <memory>

template<typename T>
class MyContainer {
private:
    T *arr;
    size_t size{};
    size_t capacity;

    void DeleteArr() const;

public:
    MyContainer();
    virtual ~MyContainer();
    void reserve(size_t n);
    void resize(size_t n, const T &value = T());
    void push_back(const T &value);

    T &at(size_t index);

    T &operator[](size_t index);
    const T &operator[](size_t index) const;
};

template<typename T>
MyContainer<T>::MyContainer() {
    arr = reinterpret_cast<T *>(malloc(sizeof(T)));
    capacity = 1;
}

template<typename T>
MyContainer<T>::~MyContainer() {
    DeleteArr();
}

template<typename T>
void MyContainer<T>::reserve(size_t n) {
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

template<typename T>
void MyContainer<T>::resize(size_t n, const T &value) {
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

template<typename T>
void MyContainer<T>::push_back(const T &value) {
    if (capacity == size) {
        reserve(2 * size);
    }

    new(arr + size) T(value);
    ++size;
}

template<typename T>
T &MyContainer<T>::at(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Attempt out of range access");
    }
    return arr[index];
}

template<typename T>
T &MyContainer<T>::operator[](size_t index) {
    return arr[index];
}

template<typename T>
const T &MyContainer<T>::operator[](size_t index) const {
    return arr[index];
}

template<typename T>
void MyContainer<T>::DeleteArr() const {
    for (size_t i = 0; i < this->size; ++i) {
        this->arr[i].~T();
    }

    delete[] reinterpret_cast<int8_t *>(this->arr);
}
