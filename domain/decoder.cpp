//
// Created by jiwoongDev on 2024-12-13.
//

#include "decoder.h"

int WidthDecoder::decode(std::vector<size_t>& enc) {
    int result = 0;
    for (int i = maxSlot - 1; i >= 0 ; i--) {
        if (enc[i] == 0) continue;
        result = result * width + (enc[i] - 1);
    }
    return result;
}

std::vector<int> WidthDecoder::decode_batch(std::vector<size_t> &enc) {
    using namespace std;
    vector<int> result;
    for (int i = 0 ; i < enc.size();) {
        int start = i;
        int temp = 0;
        for (; i < start + maxSlot && i < enc.size(); i++) {
            if (enc[i] == 0) continue;
            temp = temp * width + (enc[i] - 1);
        }
        result.push_back(temp);
    }
    return result;
}