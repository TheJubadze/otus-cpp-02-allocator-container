#include <iostream>
#include <map>
#include <boost/math/special_functions/factorials.hpp>
#include "MyContainer.h"
#include "MyAllocator.h"

using namespace OtusAllocator;

int main(int, char **) {
    Log::Log::Init();

//    {
//        std::map<int, double, std::less<>, MyAllocator<std::pair<const int, double>>> m;
//
//        for (int i = 0; i < 10; ++i) {
//            m[i] = boost::math::factorial<double>(i);
//        }
//
//        for (int i = 0; i < 10; ++i) {
//            std::cout << i << " " << m[i] << std::endl;
//        }
//    }

    {
        MyContainer<int, MyAllocator<int>> cont;

        for (int i = 0; i < 1; ++i) {
            cont.push_back(i);
        }

        for (int i = 0; i < 1; ++i) {
            std::cout << cont[i] << std::endl;
        }
    }
    return 0;
}

