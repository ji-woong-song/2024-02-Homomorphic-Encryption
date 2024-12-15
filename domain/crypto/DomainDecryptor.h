//
// Created by jiwoongDev on 2024-12-13.
//

#ifndef SEAL_REAL_DOMAINDECRYPTOR_H
#define SEAL_REAL_DOMAINDECRYPTOR_H
#include "seal/seal.h."
#include "../transform/decoder.h"
#include <vector>

class DomainDecryptor {
private:
    seal::SEALContext& context;
    Decoder& decoder;
    seal::Decryptor& decryptor;
public:
    DomainDecryptor(seal::SEALContext& context, Decoder& decoder, seal::Decryptor& decryptor)
    : context(context), decoder(decoder), decryptor(decryptor) {}

    int decrypte_single(const std::vector<seal::Ciphertext>& age);
    std::vector<int> decrypte_multiple(const std::vector<std::vector<seal::Ciphertext>>& skill);
};


#endif //SEAL_REAL_DOMAINDECRYPTOR_H
