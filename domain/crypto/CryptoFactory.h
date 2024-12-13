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
#include <memory>

class CryptoFactory {
private:
    seal::SEALContext& context;
    std::unique_ptr<Encoder> encoder;
    std::unique_ptr<Decoder> decoder;

public:
    CryptoFactory(seal::SEALContext& context, int widthPerEncode, int maxSlot) : context(context) {
        this->encoder = std::make_unique<WidthEncoder>(widthPerEncode, maxSlot);
        this->decoder = std::make_unique<WidthDecoder>(widthPerEncode, maxSlot);
    }

    DomainEncryptor createEncryptor(seal::PublicKey publicKey);
    DomainDecryptor createDecryptor(seal::SecretKey secretKey);
};


#endif //SEAL_REAL_CRYPTOFACTORY_H
