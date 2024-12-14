//
// Created by jiwoongDev on 2024-12-14.
//

#ifndef SEAL_REAL_DOMAINEVALUATOR_H
#define SEAL_REAL_DOMAINEVALUATOR_H

#include "seal/seal.h"
#include <iostream>

class DomainEvaluator {
private:
    seal::SEALContext& context;
    seal::RelinKeys relinKeys;
    seal::Evaluator evaluator;
    seal::Encryptor encryptor;
    int domain;
    int width_per_slot;
public:
    DomainEvaluator(
        seal::SEALContext& context,
        seal::RelinKeys relinKeys,
        seal::PublicKey& publicKey,
        int width_per_slot,
        int domain
    ):
    context(context),relinKeys(relinKeys),evaluator(seal::Evaluator(context)),
    encryptor(seal::Encryptor(context, publicKey)),
    width_per_slot(width_per_slot), domain(domain) {}
    seal::Ciphertext evaluate_matching_circuit(const size_t& target, const size_t& c, seal::Ciphertext& origin);
    seal::Ciphertext evaluate_intersection(std::vector<seal::Ciphertext>& cipertexts);
    seal::Ciphertext evaluate_union(std::vector<seal::Ciphertext>& cipertexts);
};


#endif //SEAL_REAL_DOMAINEVALUATOR_H
