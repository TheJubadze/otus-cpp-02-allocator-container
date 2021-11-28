#include <iostream>
#include <map>
#include <boost/math/special_functions/factorials.hpp>
#include "MyContainer.h"
#include "MyAllocator.h"

using namespace OtusAllocator;

int main(int, char **) {
    Log::Log::Init();

    auto m = std::map<int, double, std::less<>, MyAllocator<std::pair<int, double>>>(
            MyAllocator<std::pair<int, double>>());
    for (int i = 0; i < 10; ++i) {
        m[i] = boost::math::factorial<double>(i);
    }

    for (int i = 0; i < 10; ++i) {
        std::cout << i << " " << m[i] << std::endl;
    }

    MyContainer<int, MyAllocator<int>> cont{MyAllocator<int>()};
    for (int i = 0; i < 100; ++i) {
        cont.push_back(i);
    }

    for (int i = 0; i < 100; ++i) {
        std::cout << cont[i] << std::endl;
    }

    return 0;
}

