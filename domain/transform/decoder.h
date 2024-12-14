//
// Created by jiwoongDev on 2024-12-13.
//

#ifndef INC_2024_02_HOMOMORPHIC_ENCRYPTION_DECODER_H
#define INC_2024_02_HOMOMORPHIC_ENCRYPTION_DECODER_H

#include <vector>

class Decoder {
public:
    virtual int decode(std::vector<size_t>& enc) = 0;
//    virtual std::vector<int> decode_batch(std::vector<size_t>& enc) = 0;
};

class WidthDecoder : public Decoder{
private:
    int width;
public:
    WidthDecoder(int width, int maxWidth) {
        this->width = width;
    }
    int decode(std::vector<size_t> &enc) override;
//    std::vector<int> decode_batch(std::vector<size_t> &enc) override;
};

#endif //INC_2024_02_HOMOMORPHIC_ENCRYPTION_DECODER_H
