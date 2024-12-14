//
// Created by jiwoongDev on 2024-12-14.
//

#include "DomainEvaluator.h"

seal::Ciphertext DomainEvaluator::evaluate_matching_circuit(const size_t& target, const size_t& c, seal::Ciphertext& X) {
    using namespace std;
    using namespace seal;

    Ciphertext result;
    std::vector<Ciphertext> terms;
    for (int i = 1; i <= domain; i++) {
        if (i == target) continue;

        Plaintext plaintext(1);
        plaintext[0] = i;
        Ciphertext encry_i, encry_x_i;
        encryptor.encrypt(plaintext, encry_i);

        if (i > target) {
            evaluator.sub(encry_i, X, encry_x_i);
        } else {
            evaluator.sub(X, encry_i, encry_x_i);
        }
        terms.push_back(encry_x_i);
    }
    evaluator.multiply_many(terms, relinKeys, result);

    Plaintext plain_c(1);
    Ciphertext encrypt_c;
    plain_c[0] = c;
    encryptor.encrypt(plain_c, encrypt_c);
    evaluator.multiply_inplace(result, encrypt_c);
    evaluator.relinearize_inplace(result, relinKeys);
    return result;
}


seal::Ciphertext DomainEvaluator::evaluate_intersection(std::vector<seal::Ciphertext>& cipertexts) {
    using namespace std;
    using namespace seal;

    Ciphertext result;
    evaluator.multiply_many(cipertexts, relinKeys, result);
    return result;
}

seal::Ciphertext DomainEvaluator::evaluate_union(std::vector<seal::Ciphertext>& cipertexts) {
    using namespace std;
    using namespace seal;

    Ciphertext result;
    evaluator.add_many(cipertexts, result);
    return result;
}
