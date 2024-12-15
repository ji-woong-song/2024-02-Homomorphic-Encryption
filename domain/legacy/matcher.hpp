
//
// Created by jiwoong_dev on 2024-12-06.
//

#ifndef SEAL_REAL_MATCHER_HPP
#define SEAL_REAL_MATCHER_HPP

#include <iostream>
#include <vector>
#include "seal/seal.h"
#include "../../util/util.h"
#include "../data.h"
#include "../Monitor.hpp"

using namespace std;
using namespace seal;

class Matcher {
private:
    int width;
    size_t plain_modulus;
    std::vector<size_t>& c_vector;

    void debug(seal::Plaintext& plain_c, seal::SEALContext& context) {
        std::cout << "meta check " << is_metadata_valid_for(plain_c, context) << "\n";

        if (plain_c.is_ntt_form()) {
            auto context_data_ptr = context.get_context_data(plain_c.parms_id());
            auto &parms = context_data_ptr->parms();
            auto &coeff_modulus = parms.coeff_modulus();
            size_t coeff_modulus_size = coeff_modulus.size();

            const seal::Plaintext::pt_coeff_type *ptr = plain_c.data();
            for (size_t j = 0; j < coeff_modulus_size; j++) {
                uint64_t modulus = coeff_modulus[j].value();
                size_t poly_modulus_degree = parms.poly_modulus_degree();
                for (; poly_modulus_degree--; ptr++) {
                    std::cout << "*ptr >= modulus ntt form " << (*ptr >= modulus) << "\n";
                }
            }
        } else {
            auto &parms = context.first_context_data()->parms();
            uint64_t modulus = parms.plain_modulus().value();
            const seal::Plaintext::pt_coeff_type *ptr = plain_c.data();
            auto size = plain_c.coeff_count();
            for (size_t k = 0; k < size; k++, ptr++) {
                std::cout << "Index: " << k << ", Value: " << *ptr << ", Modulus: " << modulus
                          << ", (*ptr >= modulus): " << (*ptr >= modulus) << "\n";
            }
        }
    }
public:
    Matcher(const int& width,std::vector<size_t>& c_vector,  size_t& plain_modulus)
    :width(width), plain_modulus(plain_modulus), c_vector(c_vector) {
        if (c_vector.empty()) {
            c_vector.push_back(1);
            for (size_t target = 1; target <= width; target++) {
                size_t c = get_inverse_element(target, width, plain_modulus);
                cout << c << "\n";
                c_vector.push_back(c);
            }
        }
    }

    Ciphertext match_with_enc(
            const size_t& target,
            Ciphertext& X,
            MatchingParam& param,
            Monitor& monitor,
            bool debug = false
    ){
        size_t c = c_vector[target];
        Encryptor& encryptor = param.encryptor;
        Evaluator& evaluator = param.evaluator;

        seal::Plaintext plain_c(1);
        plain_c[0] = c;
        Ciphertext encrypted_c;
        encryptor.encrypt(plain_c, encrypted_c);
        if (debug) {
            this->debug(plain_c, param.context);
        }

        std::vector<Ciphertext> terms;
        for (int i = 1 ; i <= width ; i++) {
            if (i == target) continue;
            Ciphertext term;
            seal::Plaintext plain_i;
            plain_i.resize(1);
            plain_i[0] = i;
            Ciphertext encrypted_i, encrypted_x_i;
            encryptor.encrypt(plain_i, encrypted_i);
            if (i > target) {
                evaluator.sub(encrypted_i, X, encrypted_x_i);
            } else {
                evaluator.sub(X, encrypted_i, encrypted_x_i);
            }
            terms.push_back(encrypted_x_i);
        }

        Ciphertext encrypted_matched;
        evaluator.multiply_many(terms, param.relinKeys, encrypted_matched);
        monitor.printNoise(encrypted_matched);
        evaluator.multiply_inplace(encrypted_matched, encrypted_c);
        evaluator.relinearize_inplace(encrypted_matched, param.relinKeys);
        monitor.debug(encrypted_matched);
        return encrypted_matched;
    }

    Ciphertext match_one_enough(
            vector<pair<size_t, size_t>>& target_vector,
            pair<Ciphertext, Ciphertext>& X,
            MatchingParam& param,
            Monitor& monitor,
            bool debug = true
    ) {
        Encryptor& encryptor = param.encryptor;
        vector<Ciphertext> cipher_vector;
        Ciphertext total;
        for (auto &target: target_vector) {
            Ciphertext ciphertext;
            Ciphertext c_f = this->match_with_enc(target.first, X.first, param, monitor, debug);
            Ciphertext c_s = this->match_with_enc(target.second, X.second, param, monitor, debug);
            param.evaluator.multiply(c_f, c_s, ciphertext);
            param.evaluator.relinearize_inplace(ciphertext, param.relinKeys);
            cipher_vector.push_back(ciphertext);
        }
        param.evaluator.add_many(cipher_vector, total);
        return total;
    }

    Ciphertext match_all(
            vector<pair<size_t, size_t>>& target_vector,
            pair<Ciphertext, Ciphertext>& X,
            MatchingParam& param,
            Monitor& monitor,
            bool debug = false
    ) {
        vector<Ciphertext> cipher_vector;
        Ciphertext total;
        for (auto &target: target_vector) {
            Ciphertext ciphertext;
            Ciphertext c_f = this->match_with_enc(target.first, X.first, param, monitor, debug);
            Ciphertext c_s = this->match_with_enc(target.second, X.second, param, monitor, debug);
            param.evaluator.multiply(c_f, c_s, ciphertext);
            param.evaluator.relinearize_inplace(ciphertext, param.relinKeys);
            cipher_vector.push_back(ciphertext);
        }
        param.evaluator.multiply_many(cipher_vector, param.relinKeys, total);
        return total;
    }
};

#endif //SEAL_REAL_MATCHER_HPP