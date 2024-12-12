//
// Created by jiwoong_dev on 2024-12-06.
//

#include "util.h"
#include <iostream>
#include <fstream>

size_t get_inverse_element(size_t& target, int width, size_t& plain_modulus) {
    size_t numerator = get_numerator(target, width, plain_modulus);
    size_t c = 0;
    for (size_t i = 1; i < plain_modulus; i++) {
        if ((numerator * i) % plain_modulus == 1) {
            c = i;
            break;
        }
    }
    std::cout << "numerator : " << numerator << "c : " << c << "(numerator * c) % plain_modulus " << (numerator * c) % plain_modulus <<  "\n";

    return c;
}

size_t get_numerator(const size_t &target, int width, size_t& plain_modulus) {
    size_t numerator = 1;
    for (int i = 1; i <= width; i++) {
        if (i == target) continue;

        if (i < target) {
            numerator *= (target - i);
        } else if (i > target) {
            numerator *= (i - target);
        }
    }
    std::cout << "numerator : " << numerator << "\n";
    return numerator;
}

seal::SEALContext load_context(
        const size_t & poly_modulus_degree,
        const size_t & plain_modulus_value,
        seal::scheme_type type
) {
    using namespace std;
    using namespace seal;
    //43046721;

    EncryptionParameters parms(type);
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_plain_modulus(plain_modulus_value);
    parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree, {60, 60, 60, 60, 60}));

    SEALContext context(parms);
    if (!context.parameters_set()) {
        cout << "파라미터 설정에 실패했습니다. 파라미터를 확인하세요." << endl;
        auto error = context.parameter_error_message();
        cout << "에러 메시지: " << error << endl;
    }


    for (const auto &coeff_prime : parms.coeff_modulus()) {
        uint64_t gcd_value = gcd(plain_modulus_value, coeff_prime.value());
        if (gcd_value != 1) {
            cout << "경고: 평문 모듈러스와 계수 모듈러스의 소수 "
                 << coeff_prime.value() << "이(가) 서로소가 아닙니다. GCD: " << gcd_value << endl;
        }
    }
    cout << "평문 모듈러스와 계수 모듈러스가 호환됩니다." << endl;
    return context;
}


void key_save(seal::PublicKey& publicKey, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "public key fail\n";
        throw std::runtime_error("file open fail " + filename);
    }
    publicKey.save(out);
    out.close();
}

void key_save(seal::SecretKey& secretKey, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "secret key fail\n";
        throw std::runtime_error("file open fail " + filename);
    }
    secretKey.save(out);
    out.close();
}

void key_save(seal::RelinKeys& relinKeys, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "relinKeys key fail\n";
        throw std::runtime_error("file open fail " + filename);
    }
    relinKeys.save(out);
    out.close();
}

void key_load(seal::PublicKey& publicKey, seal::SEALContext& context, std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "public key fail\n";
        throw std::runtime_error("file open fail " + filename);
    }
    publicKey.load(context, in);
    in.close();
}

void key_load(seal::SecretKey& secretKey, seal::SEALContext& context, std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "secret key fail\n";
        throw std::runtime_error("file open fail " + filename);
    }
    secretKey.load(context, in);
    in.close();
}

void key_load(seal::RelinKeys& relinKeys, seal::SEALContext& context, std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "relinKeys key fail\n";
        throw std::runtime_error("file open fail " + filename);
    }
    relinKeys.load(context, in);
    in.close();
}


void cipher_load(seal::Ciphertext& ciphertext, seal::SEALContext& context, const std::string& filename) {

}

void cipher_save(seal::Ciphertext& ciphertext, std::string& filename) {
    std::ofstream age_file(filename, std::ios::binary);
    if (!age_file) {
        throw std::runtime_error("파일을 열 수 없습니다: " + filename);
    }
    ciphertext.save(age_file);
    age_file.close();
}

seal::Ciphertext encrypt(size_t data, seal::Encryptor &encryptor) {
    seal::Plaintext plaintext(1);
    plaintext[0] = data;
    seal::Ciphertext ciphertext;
    encryptor.encrypt(plaintext, ciphertext);
    return ciphertext;
}