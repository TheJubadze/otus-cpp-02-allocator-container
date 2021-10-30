#include <cstdlib>
#include <memory>
#include "MyContainer.h"

template<typename T>
void MyContainer<T>::reserve(size_t n) {
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

    size_t i = 0;

//    try {
//        for (i = 0; i < size; ++i) {
//            newarr[i] = new(newarr + i) T(arr[i]);
//        }
//    } catch (...) {
//        for (size_t j = 0; j < i; ++j) {
//            newarr[i].~T();
//        }
//        delete[] reinterpret_cast<int8_t *>(newarr);
//        throw;
//    }

    for (i = 0; i < size; ++i) {
        arr[i].~T();
    }

    delete[] reinterpret_cast<int8_t *>(arr);
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
        reserve(1.6 * size);
    }

    arr[size] = new(arr + size) T(value);
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
