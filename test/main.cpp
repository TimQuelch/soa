#include <iostream>

#include "soa.h"

int main() {
    auto s = soa::soa<int, double, float>{};
    s.push_back({1, 1.2, 2.3f});
    s.push_back({5, 6, 7});
    //const auto s2 = s;
    //auto a = s[0];
    //auto b = s2[1];

    //std::cout << std::get<0>(a) << std::get<1>(a) << std::get<2>(a) << std::endl;
    //std::cout << std::get<0>(b) << std::get<1>(b) << std::get<2>(b) << std::endl;

    return 0;
}
