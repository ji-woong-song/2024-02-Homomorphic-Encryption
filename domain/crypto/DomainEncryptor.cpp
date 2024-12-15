//
// Created by jiwoongDev on 2024-12-13.
//

#include "DomainEncryptor.h"

std::vector<seal::Ciphertext> DomainEncryptor::encrypte_single(int age) {
    using namespace seal;
    std::vector<size_t> encoded_age = encoder.encode(age);
    std::vector<seal::Ciphertext> vec;
    for (size_t enc: encoded_age) {
        Plaintext plaintext(1);
        plaintext[0] = enc;
        Ciphertext ciphertext;
        encryptor.encrypt(plaintext, ciphertext);
        vec.push_back(ciphertext);
    }
    return vec;
}

std::vector<std::vector<seal::Ciphertext>> DomainEncryptor::encrypte_multiple(std::vector<int>& skills) {
    using namespace std;
    using namespace seal;

    std::vector<std::vector<seal::Ciphertext>> vec;
    for (int skill : skills) {
        vec.push_back(encrypte_single(skill));
    }
    return vec;
}
