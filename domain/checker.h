//
// Created by jiwoongDev on 2024-12-14.
//

#ifndef SEAL_REAL_CHECKER_H
#define SEAL_REAL_CHECKER_H


#include "transform/encoder.h"
#include "transform/decoder.h"
#include "crypto/DomainEncryptor.h"
#include "crypto/DomainEvaluator.h"
#include "calculator/calculator.h"
#include "Monitor.hpp"

class Checker {
private:
    seal::SEALContext& context;
    DomainEvaluator& evaluator;
    DomainEncryptor& encryptor;
    Encoder& encoder;
    Calculator& calculator;
    Monitor& monitor;
    int max_width;
public:
    Checker(seal::SEALContext& context,Encoder& encoder, DomainEncryptor& encryptor, DomainEvaluator& evaluator, Calculator& calculator, Monitor& monitor, int max_width)
    :context(context), encoder(encoder), evaluator(evaluator), encryptor(encryptor), calculator(calculator), monitor(monitor), max_width(max_width) {}
    
    seal::Ciphertext check_age(int min_age, int max_age, std::vector<seal::Ciphertext>& encrypted_age);
    seal::Ciphertext check_skills(const std::vector<int>& skills, std::vector<std::vector<seal::Ciphertext>>& encrypted_skills);
};


#endif //SEAL_REAL_CHECKER_H
