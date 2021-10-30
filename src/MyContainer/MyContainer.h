#pragma once

template<typename T>
class MyContainer {
private:
    T *arr;
    size_t size;
    size_t capacity;

public:
    void reserve(size_t n);
    void resize(size_t n, const T &value = T());
    void push_back(const T &value);

    T &at(size_t index);

    T &operator[](size_t index);
    const T &operator[](size_t index) const;
};

