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
        friend
        class MyAllocator;
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
        free(m_data); // ??????????????? Tries to access an object by deleted pointer ()

//        std::_Container_base12::_Orphan_all_unlocked_v3() xmemory:1243
//        std::_Container_base12::_Orphan_all_locked_v3() xmemory:1097
//        std::_Container_base12::_Orphan_all() xmemory:1257
//        std::_Tree_val::_Erase_head<…>(OtusAllocator::MyAllocator<…> &) xtree:757
//        std::_Tree::~_Tree<…>() xtree:1094
//                                      <unknown> 0x00007ff67eafee64
//        main(int, char **) main.cpp:32
//        invoke_main() 0x00007ff67eb5de79
//        __scrt_common_main_seh() 0x00007ff67eb5dd5e
//        __scrt_common_main() 0x00007ff67eb5dc1e
//        mainCRTStartup(void *) 0x00007ff67eb5df0e
//                               <unknown> 0x00007ffba5e77bd4
//                               <unknown> 0x00007ffba69cce51

    }

}