//
// Created by jiwoong_dev on 2024-12-08.
//

#ifndef SEAL_REAL_MATCHINGSERVICE_HPP
#define SEAL_REAL_MATCHINGSERVICE_HPP

#include "respository.hpp"
#include "matcher.hpp"

class MatchingService {
private:
    Monitor& monitor;
    Repository& repository;
    Matcher& matcher;
    seal::SEALContext& context;

public:
    MatchingService(Monitor& monitor, Repository& repository, Matcher& matcher, seal::SEALContext& context)
    : monitor(monitor), repository(repository), matcher(matcher), context(context) {}

    void filter(
            vector<EmployeeData>& employeeData,
            PublicKey& publicKey,
            RelinKeys& relinKeys,
            vector<std::pair<size_t, size_t>>& age_cond,
            vector<std::pair<size_t, size_t>>& skill_cond
    ) {
        Encryptor encryptor(context, publicKey);
        Evaluator evaluator(context);
        Plaintext one(1), zero(1);
        one[0] = 1;
        zero[0] = 1;
        Ciphertext one_cipher, zero_cipher;
        encryptor.encrypt(one, one_cipher);
        encryptor.encrypt(zero, zero_cipher);

        MatchingParam param = {context, encryptor, relinKeys, evaluator, one_cipher, zero_cipher};
        for (EmployeeData& data : employeeData) {
            pair<seal::Ciphertext, seal::Ciphertext> &age = data.age;
            vector<std::pair<seal::Ciphertext, seal::Ciphertext>> &skills = data.skills;

            const Ciphertext age_condition = matcher.match_one_enough(age_cond, age, param, monitor);
            vector<Ciphertext> skill_condition;
            for (auto & skill : skills) {
                const Ciphertext ciphertext = matcher.match_all(skill_cond, skill, param, monitor);
                skill_condition.push_back(ciphertext);
            }

            Ciphertext total;
            evaluator.multiply_many(skill_condition, relinKeys, total);
            evaluator.multiply_inplace(total, age_condition);
            evaluator.relinearize_inplace(total, relinKeys);

            evaluator.multiply_inplace(data.age.first, total);
            evaluator.relinearize_inplace(data.age.first, relinKeys);
            evaluator.multiply_inplace(data.age.second, total);
            evaluator.relinearize_inplace(data.age.second, relinKeys);

            for (auto & skill : skills) {
                evaluator.multiply_inplace(skill.first, total);
                evaluator.multiply_inplace(skill.first, total);
                evaluator.multiply_inplace(skill.second, total);
                evaluator.multiply_inplace(skill.second, total);
            }
        }
    }
    vector<EmployeeData> get_filtered(
            std::string & employer_id,
            vector<std::pair<size_t, size_t>>& age_cond,
            vector<std::pair<size_t, size_t>>& skill_cond
        ) {
        vector<EmployeeData> employeeDataList = repository.load_all_employee_data(employer_id);
        PublicKey publicKey = repository.load_public_key(employer_id);
        RelinKeys relinKeys = repository.load_relin_key(employer_id);

        filter(employeeDataList, publicKey, relinKeys, age_cond, skill_cond);
        return employeeDataList;
    }
};



#endif //SEAL_REAL_MATCHINGSERVICE_HPP
