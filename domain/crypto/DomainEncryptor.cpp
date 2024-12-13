//
// Created by jiwoongDev on 2024-12-13.
//

#include "DomainEncryptor.h"

seal::Ciphertext DomainEncryptor::encrypteAge(int age) {
    using namespace seal;
    std::vector<size_t> encoded_age = encoder.encode(age);
    BatchEncoder batchEncoder(context);
    Plaintext plaintext;
    batchEncoder.encode(encoded_age, plaintext);
    Ciphertext ciphertext;
    encryptor.encrypt(plaintext, ciphertext);
    return ciphertext;
}

seal::Ciphertext DomainEncryptor::encrypteSkills(std::vector<int>& skills) {
    using namespace std;
    using namespace seal;
    vector<size_t> encoded_skill;
    for (int skill : skills) {
        const vector <size_t> &res = encoder.encode(skill);
        for (const size_t& item: res) {
            encoded_skill.push_back(item);
        }
    }
    BatchEncoder batchEncoder(context);
    Plaintext plaintext;
    batchEncoder.encode(encoded_skill, plaintext);
    Ciphertext ciphertext;
    encryptor.encrypt(plaintext, ciphertext);
    return ciphertext;
}
