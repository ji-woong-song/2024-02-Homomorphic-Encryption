//
// Created by jiwoong_dev on 2024-12-08.
//

#ifndef SEAL_REAL_DATA_H
#define SEAL_REAL_DATA_H
#include <iostream>
#include "seal/seal.h"

struct PublicKeyEntry {
    std::string id;
    seal::PublicKey pub_key;
};

struct EmployeeData {
    int id;
    std::pair<seal::Ciphertext, seal::Ciphertext> age;
    std::vector<std::pair<seal::Ciphertext, seal::Ciphertext>> skills;
};

struct MatchingParam {
    seal::SEALContext& context;
    seal::Encryptor& encryptor;
    seal::RelinKeys& relinKeys;
    seal::Evaluator& evaluator;
    seal::Ciphertext& one;
    seal::Ciphertext& zero;
};
#endif //SEAL_REAL_DATA_H
