//
// Created by jiwoongDev on 2024-12-13.
//

#ifndef SEAL_REAL_DOMAINENCRYPTOR_H
#define SEAL_REAL_DOMAINENCRYPTOR_H

#include "seal/seal.h"
#include "../encoder.h"
#include <vector>

class DomainEncryptor {
private:
    seal::SEALContext& context;
    seal::Encryptor encryptor;
    Encoder& encoder;
public:
    DomainEncryptor(seal::SEALContext& context, Encoder& encoder, seal::PublicKey publicKey)
    : context(context), encoder(encoder), encryptor(seal::Encryptor(context, publicKey)) {}
    seal::Ciphertext encrypteAge(int age);
    seal::Ciphertext encrypteSkills(std::vector<int>& skills);
};


#endif //SEAL_REAL_DOMAINENCRYPTOR_H
