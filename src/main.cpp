#include <iostream>
#include <map>
#include <boost/math/special_functions/factorials.hpp>
#include "MyContainer.h"
#include "MyAllocator.h"
#include "FixedAllocator.h"

using namespace OtusAllocator;

int main(int, char **) {
    Log::Log::Init();

    {
        const size_t capacity = 10;
        std::map<int, double, std::less<>, FixedAllocator<std::pair<const int, double>, capacity * 2>> m;

        for (int i = 0; i < 10; ++i) {
            m[i] = boost::math::factorial<double>(i);
        }

        for (int i = 0; i < 10; ++i) {
            std::cout << i << " " << m[i] << std::endl;
        }
    }

    MY_INFO("===============================================================");

    {
        const size_t capacity = 1024;
        MyContainer<long double, FixedAllocator<long double, capacity * 2>> cont;

        for (int i = 0; i < capacity; ++i) {
            cont.push_back(i + 101);
        }

        for (int i = 0; i < capacity; ++i) {
            std::cout << cont[i] << std::endl;
        }
    }
    return 0;
}

