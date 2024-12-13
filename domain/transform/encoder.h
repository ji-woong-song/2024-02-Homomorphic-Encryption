//
// Created by jiwoongDev on 2024-12-13.
//

#ifndef INC_2024_02_HOMOMORPHIC_ENCRYPTION_ENCODER_H
#define INC_2024_02_HOMOMORPHIC_ENCRYPTION_ENCODER_H

#include <vector>

class Encoder {
public:
    virtual std::vector<size_t> encode(int n) = 0;
};

class WidthEncoder : public Encoder {
private:
    int width;
    int maxSlot;
public:
    WidthEncoder(int width, int maxSlot) {
        this->width = width;
        this->maxSlot = maxSlot;
    }

    std::vector<size_t> encode(int n) override;
};

#endif //INC_2024_02_HOMOMORPHIC_ENCRYPTION_ENCODER_H
