//
// Created by jiwoongDev on 2024-12-13.
//

#include "CryptoFactory.h"

DomainEncryptor CryptoFactory::createEncryptor(seal::Encryptor& encryptor) {
    return {context, *encoder, encryptor};
}

DomainDecryptor CryptoFactory::createDecryptor(seal::Decryptor& decryptor) {
    return {context, *decoder, decryptor};
}

DomainEvaluator CryptoFactory::createEvaluator(seal::Evaluator& evaluator, seal::RelinKeys& relinKeys, seal::Encryptor& encryptor) {
    return {context, relinKeys, evaluator, encryptor, width_per_encode };
}

Checker CryptoFactory::createChecker(
        DomainEvaluator &evaluator, DomainEncryptor &encryptor,
        Calculator& calculator, std::vector<int>& c_vector
) {
    return {context, *encoder, encryptor, evaluator, calculator,c_vector, width_per_encode };
}

Calculator CryptoFactory::createCalculator() {
    return {context, width_per_encode};
}