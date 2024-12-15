//
// Created by jiwoong_dev on 2024-12-06.
//

#ifndef SEAL_REAL_MONITOR_H
#define SEAL_REAL_MONITOR_H
#include "seal/seal.h"
#include <iostream>

class Monitor {
private:
    seal::Decryptor& decryptor;
    size_t& plain_modulus_value;
public:
    Monitor(seal::Decryptor &decryptor, size_t & plain_modulus_value) :
            decryptor(decryptor), plain_modulus_value(plain_modulus_value) {
    }
    void printNoise(seal::Ciphertext& ciphertext) {
        std::cout << "noise :" << this->decryptor.invariant_noise_budget(ciphertext) << " bits" << std::endl;
    }

    void debug(seal::Ciphertext &ciphertext) {
        seal::Plaintext plain_result;
        decryptor.decrypt(ciphertext, plain_result);
        // 결과 출력
        size_t result = stoull(plain_result.to_string()) % plain_modulus_value;
        std::cout << "evaluation result : " << result << std::endl;
    }
};

#endif //SEAL_REAL_MONITOR_H
