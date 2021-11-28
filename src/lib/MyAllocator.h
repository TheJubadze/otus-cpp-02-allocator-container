#pragma once

namespace OtusAllocator {

    template<typename T>
    class MyAllocator {
    public:
        using value_type = T;

        MyAllocator();

        template<typename U>
        explicit MyAllocator(const MyAllocator<U> &right);

        T *allocate(size_t count) const;

        void deallocate(T *ptr, size_t n);

        template<typename... TArgs>
        void construct(T *ptr, const TArgs &... args);

        void destroy(T *ptr);
    };

}