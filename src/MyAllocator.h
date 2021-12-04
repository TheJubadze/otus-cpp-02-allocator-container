#pragma once

#include "Log.h"

namespace OtusAllocator {

    template<typename T>
    class MyAllocator {
    public:
        using value_type = T;

        MyAllocator();
        virtual ~MyAllocator();

        template<typename U>
        explicit MyAllocator(const MyAllocator<U> &right);

        T *allocate(size_t count) const;

        void deallocate(T *ptr, size_t n);

        template<typename... TArgs>
        void construct(T *ptr, const TArgs &... args);

        void destroy(T *ptr);

    private:
        const int OneMb = 1048576;
        char *m_data = nullptr;
        mutable char *m_current = nullptr;
        mutable size_t m_inUse = 0;

        template<typename>
        friend class MyAllocator;
    };

    template<typename T>
    MyAllocator<T>::MyAllocator() {
        auto mem = malloc(OneMb);
        m_current = m_data = reinterpret_cast<char *>(mem);
    }

    template<typename T>
    template<typename U>
    MyAllocator<T>::MyAllocator(const MyAllocator<U> &right)
            : MyAllocator() {
        memcpy(m_data, right.m_data, m_inUse);
        m_current += right.m_inUse;
    };

    template<typename T>
    T *MyAllocator<T>::allocate(size_t count) const {
        MY_CORE_TRACE("Allocating: {} x {} = {} bytes", count, sizeof(T), count * sizeof(T));
        auto result = m_current;
        auto bytes = count * sizeof(T);
        m_current += bytes;
        m_inUse += bytes;
        return reinterpret_cast<T *>(result);
    }

    template<typename T>
    void MyAllocator<T>::deallocate(T *ptr, size_t n) {
        m_current -= n;
        m_inUse -= n;
        MY_CORE_TRACE("{} bytes deallocated", n);
    }

    template<typename T>
    template<typename... TArgs>
    void MyAllocator<T>::construct(T *ptr, const TArgs &... args) {
        MY_CORE_TRACE("Constructing");
        new(ptr) T(args...);
        MY_CORE_TRACE("Constructed");
    }

    template<typename T>
    void MyAllocator<T>::destroy(T *ptr) {
        MY_CORE_TRACE("Destroying");
        ptr->~T();
        MY_CORE_TRACE("Destroyed");
    }

    template<typename T>
    MyAllocator<T>::~MyAllocator() {
        //free(m_data); ??????????????? Tries to access an object by deleted pointer
    }

}