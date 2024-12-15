//
// Created by jiwoongDev on 2024-12-13.
//

#include "encoder.h"

std::vector<size_t> WidthEncoder::encode(int n) {
    std::vector<size_t> result;

    for (int i = 0 ; i < maxSlot; i++) {
        int e = n % width + 1;
        result.push_back(e);
        n /= width;
    }
    return result;
}