//
// Created by jiwoongDev on 2024-12-13.
//

#include "CryptoFactory.h"

DomainEncryptor CryptoFactory::createEncryptor(seal::PublicKey publicKey) {
    using namespace seal;
    return {context, *encoder, publicKey};
}

DomainDecryptor CryptoFactory::createDecryptor(seal::SecretKey secretKey) {
    return {context, *decoder, secretKey};
}

DomainEvaluator CryptoFactory::createEvaluator(seal::PublicKey publicKey, seal::RelinKeys relinKeys) {
    return {context, relinKeys, publicKey, width_per_encode, max_slot };
}
