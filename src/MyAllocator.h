#pragma once

#include "Log.h"

namespace OtusAllocator {

    template<typename T>
    class MyAllocator {
    public:
        using value_type = T;

        MyAllocator();
        virtual ~MyAllocator() = default;

        T *allocate(size_t count) const;

        void deallocate(T *ptr, size_t n);

        template<typename... TArgs>
        void construct(T *ptr, const TArgs &... args);

        void destroy(T *ptr);

    private:
        const int OneMb = 8; //1048576;
        char *m_data = nullptr;
        mutable char *m_current = nullptr;
        mutable size_t m_inUse = 0;

        template<typename>
        friend
        class MyAllocator;
    };

    template<typename T>
    MyAllocator<T>::MyAllocator() {
        MY_TRACE("A >>>> Allocator() ctor");
        auto mem = malloc(OneMb);
        m_current = m_data = reinterpret_cast<char *>(mem);
    }

    template<typename T>
    T *MyAllocator<T>::allocate(size_t count) const {
        MY_TRACE("A >>>> Allocating: {} x {} = {} bytes", count, sizeof(T), count * sizeof(T));
        auto result = m_current;
        auto bytes = count * sizeof(T);
        m_current += bytes;
        m_inUse += bytes;
        MY_TRACE("A >>>> {} b allocated, {} b in use", bytes, m_inUse);
        return reinterpret_cast<T *>(result);
    }

    template<typename T>
    void MyAllocator<T>::deallocate(T *ptr, size_t n) {
        auto bytes = n * sizeof(T);
        MY_TRACE("A >>>> Deallocating {} b, in use {} b", bytes, m_inUse);
        m_current -= bytes;
        m_inUse -= bytes;
        MY_TRACE("A >>>> {} b deallocated, {} b in use", bytes, m_inUse);
        if (m_inUse <= 0) {
            std::free(m_data);
            MY_TRACE("A >>>> Mempool freed!");
        }
    }

    template<typename T>
    template<typename... TArgs>
    void MyAllocator<T>::construct(T *ptr, const TArgs &... args) {
        MY_TRACE("A >>>> Constructing");
        new(ptr) T(args...);
        MY_TRACE("A >>>> Constructed");
    }

    template<typename T>
    void MyAllocator<T>::destroy(T *ptr) {
        MY_TRACE("A >>>> Destroying");
        ptr->~T();
        MY_TRACE("A >>>> Destroyed");
    }
}