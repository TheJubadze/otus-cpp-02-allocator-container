#pragma once

#include <cstdlib>
#include <memory>
#include "Log.h"

namespace OtusAllocator {

    template<typename T, typename TAllocator = std::allocator<T>>
    class MyContainer {
    private:
        T *arr;
        size_t m_size{};
        size_t m_capacity;
        TAllocator m_alloc = TAllocator();

        using AllocatorTraits = std::allocator_traits<TAllocator>;

        void DeleteArr();

    public:
        explicit MyContainer();
        explicit MyContainer(size_t, const T &value = T(), TAllocator &allocator = TAllocator());
        virtual ~MyContainer();

        void reserve(size_t n);
        void resize(size_t n, const T &value = T());
        void push_back(const T &value);

        T &at(size_t index);
        T &operator[](size_t index);
        const T &operator[](size_t index) const;
    };

    template<typename T, typename TAllocator>
    MyContainer<T, TAllocator>::MyContainer()
        : m_capacity(1) {
        arr = AllocatorTraits::allocate(m_alloc, 1);
    }

    template<typename T, typename TAllocator>
    MyContainer<T, TAllocator>::MyContainer(size_t n, const T &value, TAllocator &allocator)
        : MyContainer(allocator) {
        m_size = n;
        arr[m_size - 1] = value;
    }

    template<typename T, typename TAllocator>
    MyContainer<T, TAllocator>::~MyContainer() {
        DeleteArr();
    }

    template<typename T, typename TAllocator>
    void MyContainer<T, TAllocator>::reserve(size_t n) {
        MY_TRACE("Reserving {}. Capacity = {}", n, m_capacity);
        if (n <= m_capacity) {
            return;
        }

        MY_TRACE("Allocating: {} x {} = {} bytes", n, sizeof(T), n * sizeof(T));
        T *newArr = AllocatorTraits::allocate(m_alloc, n);
        MY_TRACE("{} bytes allocated", n * sizeof(T));

        try {
            MY_TRACE("uninitialized_copy starting");
            std::uninitialized_copy(arr, arr + m_size, newArr);
            MY_TRACE("uninitialized_copy success");
        } catch (...) {
            MY_ERROR("uninitialized_copy failed");
            MY_TRACE("Deallocating {} elements...", m_size);
            AllocatorTraits::deallocate(m_alloc, newArr, m_size);
            MY_TRACE("{} elements deallocated", m_size);
            throw;
        }

        DeleteArr();

        arr = newArr;
        m_capacity = n;
    }

    template<typename T, typename TAllocator>
    void MyContainer<T, TAllocator>::resize(size_t n, const T &value) {
        MY_TRACE("Resizing {}. Capacity = {}", n, m_capacity);
        if (n > m_capacity) {
            reserve(n);
        }

        for (size_t i = m_size; i < n; ++i) {
            MY_TRACE("resize: Constructing {}", i);
            AllocatorTraits::construct(m_alloc, arr + i, value);
            MY_TRACE("resize: {} constructed", i);
        }

        if (n < m_size) {
            m_size = n;
        }
    }

    template<typename T, typename TAllocator>
    void MyContainer<T, TAllocator>::push_back(const T &value) {
        MY_TRACE("push_back {}. Capacity = {}. Size = {}", value, m_capacity, m_size);
        if (m_capacity == m_size) {
            reserve(2 * m_size);
        }

        MY_TRACE("push_back: Constructing {}", value);
        AllocatorTraits::construct(m_alloc, arr + m_size, value);
        MY_TRACE("push_back: {} constructed", value);
        ++m_size;
    }

    template<typename T, typename TAllocator>
    T &MyContainer<T, TAllocator>::at(size_t index) {
        if (index >= m_size) {
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
        auto t = typeid(T).name();
        MY_TRACE("Delete Arr<{}> [{}] elements", t, m_size);
        for (size_t i = 0; i < this->m_size; ++i) {
            MY_TRACE("Destroying ({})[{}] element = {}", t, i, arr[i]);
            AllocatorTraits::destroy(m_alloc, arr + i);
            MY_TRACE("({})[{}] element destroyed", t, i);
        }

        MY_TRACE("Deallocating container of ({}){} elements", t, m_size);
        AllocatorTraits::deallocate(m_alloc, arr, m_size);
        MY_TRACE("Container of ({}){} elements deallocated", t, m_size);
    }
}