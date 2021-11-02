#include <iostream>
#include "MyContainer.h"
#include "MyAllocator.h"

int main(int, char **) {
    MyContainer<int, MyAllocator<int>> cont{MyAllocator<int>()};
    for (int i = 0; i < 100; ++i) {
        cont.push_back(i);
    }

    for (int i = 0; i < 100; ++i) {
        std::cout << cont[i] << std::endl;
    }
}