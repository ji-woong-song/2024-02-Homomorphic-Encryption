//
// Created by jiwoongDev on 2024-12-13.
//

#include "encoder.h"

std::vector<size_t> WidthEncoder::encode(int n) {
    std::vector<size_t> result;
    int i = 0;
    while (i < maxSlot) {
        i++;
        if (n == 0) {
            result.push_back(0);
            continue;
        }
        int e = n % width + 1;
        n /= width;
        result.push_back(e);
    }
    return result;
}