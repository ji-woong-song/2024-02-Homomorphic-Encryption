//
// Created by jiwoongDev on 2024-12-14.
//

#include "calculator.h"

size_t Calculator::get_numerator_reverse_element(size_t x) {
    size_t numerator = get_numerator(x);
    size_t c = 0;
    for (size_t i = 1; i < plain_modulus; i++) {
        if ((numerator * i) % plain_modulus == 1) {
            c = i;
            break;
        }
    }
    std::cout << "numerator : " << numerator << " c : " << c << "(numerator * c) % plain_modulus ";
    std::cout << (numerator * c) % plain_modulus <<  "\n";
    return c;
}

size_t Calculator::get_numerator(const size_t& target) {
    size_t numerator = 1;
    for (int i = 1; i <= width; i++) {
        if (i == target) continue;
        if (i < target) {
            numerator *= (target - i);
        } else if (i > target) {
            numerator *= (i - target);
        }
    }
    std::cout << "numerator : " << numerator << "\n";
    return numerator;
}