//
// Created by jiwoongDev on 2024-12-14.
//

#ifndef SEAL_REAL_CALCULATOR_H
#define SEAL_REAL_CALCULATOR_H

#include "seal/seal.h"
#include <vector>

class Calculator {
private:
    seal::SEALContext& context;
    size_t plain_modulus;
    int width;
public:
    Calculator(seal::SEALContext& context, int width) : context(context), width(width) {
        this->plain_modulus = context.first_context_data()->parms().plain_modulus().value();
    }

    size_t get_numerator_reverse_element(size_t x);
    size_t get_numerator(const size_t& target);
    size_t get_naive_reverse_element(size_t x);
};


#endif //SEAL_REAL_CALCULATOR_H
