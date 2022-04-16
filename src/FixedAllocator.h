/// @file
/// @brief Файл с объявлением и реализацией аллокатора, выделяющего фиксированный размер памяти единоразово

#pragma once

#include <cassert>
#include <algorithm>
#include <array>

/// @brief Класс аллокатора, выделяющего фиксированный размер памяти единоразово
/// @tparam T тип объектов
/// @tparam Size размер
template<typename T, int Size>
struct FixedAllocator {
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    /// @brief Вспомогательная структура
    /// @tparam U тип
    template<typename U>
    struct rebind {
        using other = FixedAllocator<U, Size>;
    };

    /// @brief Конструктор
    FixedAllocator() = default;

    /// @brief Деструктор
    ~FixedAllocator() = default;

    /// @brief Выделение памяти
    /// @param n количество элементов
    /// @return указатель на выделенную память
    T *allocate(std::size_t n) {
        MY_TRACE("\t\t\tA >>>> Trying to allocate: {} bytes", sizeof(T) * n);
        if (n == 0) {
            return nullptr;
        } else if ((poolSize_ + n) > Size) {
            MY_ERROR("\t\t\tA >>>> pool size {} + n {} > Memory {} bytes", sizeof(T) * poolSize_, sizeof(T) * n, sizeof(T) * Size);
            throw std::bad_alloc();
        }

        T *p = nullptr;
        if (poolSize_ == 0) // Сценарий выделения новой памяти
        {
            MY_TRACE("\t\t\tA >>>> Allocating: {} bytes. pool size = 0", sizeof(T) * n);
            ptr_ = static_cast<T *>( std::malloc(sizeof(T) * Size));
            if (!ptr_) {
                MY_ERROR("\t\t\tA >>>> malloc returns null!");
                throw std::bad_alloc();
            }
            MY_TRACE("\t\t\tA >>>> {} bytes allocated", sizeof(T) * n);
            // Фиксация используемых адресов
            for (; poolSize_ < n; ++poolSize_) {
                ptrPool_[poolSize_] = ptr_ + poolSize_;
            }
            p = ptr_;
        } else // Сценарий добавления элемента в выделенную память
        {
            MY_TRACE("\t\t\tA >>>> Allocating: {} bytes. pool size = {}", sizeof(T) * n, sizeof(T) * poolSize_);
            auto it = std::find(ptrPool_.begin(), ptrPool_.end(), nullptr);
            assert(it != ptrPool_.end()); // должны быть сбободные адреса, т.к. есть проверка на размер в начале

            // Если n > 1, то элементы обязаны быть расположены в памяти последовательно
            MY_TRACE("\t\t\tA >>>> Searching for solid chunk of {} bytes...", sizeof(T) * n);
            int index = 0;
            while (it != ptrPool_.end()) {
                index = std::distance(ptrPool_.begin(), it);
                // Поиск непрерывной последовательности для всех n
                std::size_t found = 1;
                for (; found < n; ++found) {
                    if (ptrPool_[index + found] != nullptr) {
                        it = std::find(it + 1, ptrPool_.end(), nullptr);
                        break;
                    }
                }
                if (found == n) {
                    break;
                }
            }
            if (it == ptrPool_.end()) // нет последовательных адресов
            {
                MY_ERROR("\t\t\tA >>>> There is no solid chunk of memory!");
                throw std::bad_alloc();
            }

            p = ptr_ + index;
            // Фиксация используемых адресов
            for (decltype(n) i = 0; i < n; ++i) {
                ptrPool_[index + i] = ptr_ + index + i;
                ++poolSize_;
            }
            MY_TRACE("\t\t\tA >>>> {} bytes allocated", sizeof(T) * n);
        }
        return p;
    }

    /// @brief Освобождение памяти
    /// @param p указатель на память
    /// @param n количество элементов
    void deallocate(T *p, std::size_t n) {
        MY_TRACE("\t\t\tA >>>> Deallocating: {} bytes. pool size = {}", sizeof(T) * n, sizeof(T) * poolSize_);
        auto it = std::find(ptrPool_.begin(), ptrPool_.end(), p);
        assert(it != ptrPool_.end());

        // Фиксация освобожденных адресов
        MY_TRACE("\t\t\tA >>>> Freeing addresses...");
        for (decltype(n) i = 0; i < n; ++i) {
            *it = nullptr;
            --poolSize_;
            ++it;
        }
        MY_TRACE("\t\t\tA >>>> Addresses freed!");
        if (poolSize_ == 0) {
            std::free(ptr_);
            ptr_ = nullptr;
        }
        MY_TRACE("\t\t\tA >>>> {} bytes deallocated", sizeof(T) * n);
    }

    /// @brief Конструирование объекта
    /// @tparam U тип объекта
    /// @tparam ...Args типы параметров конструктора объекта
    /// @param p указатель на объект
    /// @param ...args параметры конструктора объекта
    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    /// @brief Уничтожение объекта
    /// @param p указатель на объект
    void destroy(T *p) {
        p->~T();
    }

    /// @brief Получение адреса памяти
    /// @return адрес на начало памяти
    const T *data() const {
        return ptr_;
    }

    /// @brief Получение размера используемой памяти
    /// @return размер используемой памяти
    std::size_t size() const {
        return poolSize_;
    }
private:
    T *ptr_ = nullptr;
    std::array<T *, Size> ptrPool_ = {}; // инициализируется нулями
    std::size_t poolSize_ = 0;
};
