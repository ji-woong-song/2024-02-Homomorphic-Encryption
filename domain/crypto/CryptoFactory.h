//
// Created by jiwoongDev on 2024-12-13.
//

#ifndef SEAL_REAL_CRYPTOFACTORY_H
#define SEAL_REAL_CRYPTOFACTORY_H

#include "seal/seal.h"
#include "../transform/encoder.h"
#include "../transform/decoder.h"
#include "DomainEncryptor.h"
#include "DomainDecryptor.h"
#include "DomainEvaluator.h"
#include "../checker.h"
#include <memory>

class CryptoFactory {
private:
    seal::SEALContext& context;
    std::unique_ptr<Encoder> encoder;
    std::unique_ptr<Decoder> decoder;
    int width_per_encode;
    int max_slot;
public:
    CryptoFactory(seal::SEALContext& context, int width_per_encode, int max_slot) : context(context) {
        this->encoder = std::make_unique<WidthEncoder>(width_per_encode, max_slot);
        this->decoder = std::make_unique<WidthDecoder>(width_per_encode, max_slot);
        this->width_per_encode = width_per_encode;
        this->max_slot = max_slot;
    }

    DomainEncryptor createEncryptor(seal::Encryptor& encryptor);
    DomainDecryptor createDecryptor(seal::Decryptor& decryptor);
    DomainEvaluator createEvaluator(seal::Evaluator& evaluator, seal::RelinKeys& relinKeys, seal::Encryptor& encryptor);
    Checker createChecker(DomainEvaluator& evaluator, DomainEncryptor& encryptor, Calculator& calculator, Monitor& monitor);
    Calculator createCalculator();
};


#endif //SEAL_REAL_CRYPTOFACTORY_H
