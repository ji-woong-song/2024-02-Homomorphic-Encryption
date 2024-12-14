//
// Created by jiwoongDev on 2024-12-13.
//

#include "DomainDecryptor.h"

int DomainDecryptor::decrypte_single(const std::vector<seal::Ciphertext>& age){
    using namespace std;
    using namespace seal;

    vector<size_t> vec;
    for (auto& cipher: age) {
        Plaintext plaintext;
        decryptor.decrypt(cipher, plaintext);
        vec.push_back(plaintext[0]);
    }
    int result = decoder.decode(vec);
    return result;
}

std::vector<int> DomainDecryptor::decrypte_multiple(const std::vector<std::vector<seal::Ciphertext>>& skill){
    using namespace std;
    using namespace seal;

    vector<int> result;
    for (auto& single : skill) {
        result.push_back(decrypte_single(single));
    }
    return result;
}