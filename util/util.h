//
// Created by jiwoong_dev on 2024-12-06.
//

#ifndef SEAL_REAL_UTIL_H
#define SEAL_REAL_UTIL_H
#include <tuple>
#include <string>
#include "seal/seal.h"

size_t get_numerator(const size_t &target, int width, size_t& plain_modulus);
size_t get_inverse_element(size_t& target, int width, size_t& plain_modulus);
seal::SEALContext load_context(
        const size_t & degree = 16384,
        const size_t & value = 362897,
        seal::scheme_type = seal::scheme_type::bfv
);

std::pair<size_t, size_t> encode_age(int age);
std::pair<size_t, size_t> encode_skill(std::string skill);

void key_save(seal::PublicKey& publicKey, const std::string& filename);
void key_save(seal::SecretKey& secretKey, const std::string& filename);
void key_save(seal::RelinKeys& relinKeys, const std::string& filename);

void key_load(seal::PublicKey& publicKey, seal::SEALContext& context, const std::string& filename);
void key_load(seal::SecretKey& secretKey, seal::SEALContext& context,const std::string& filename);
void key_load(seal::RelinKeys& relinKeys, seal::SEALContext& context,const std::string& filename);

void cipher_load(seal::Ciphertext& ciphertext, seal::SEALContext& context, const std::string& filename);
void cipher_save(seal::Ciphertext& ciphertext,  std::string& filename);

seal::Ciphertext encrypt(size_t data, seal::Encryptor &encryptor);

#endif //SEAL_REAL_UTIL_H
