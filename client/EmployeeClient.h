//
// Created by jiwoong_dev on 2024-12-15.
//

#ifndef SEAL_REAL_EMPLOYEECLIENT_H
#define SEAL_REAL_EMPLOYEECLIENT_H

#include "seal/seal.h"
#include <string>
#include <vector>
#include "../domain/crypto/DomainEncryptor.h"
#include "../client/SkillTable.h"
#include "../domain/crypto/CryptoFactory.h"

class EmployeeClient {
private:
    seal::SEALContext& context;
    seal::PublicKey publicKey;
    seal::SecretKey secretKey;
    seal::RelinKeys relinKeys;
    SkillTable& skillTable;

    int age;
    std::vector<std::string> skills;
public:
    EmployeeClient(seal::SEALContext& context, SkillTable& skillTable) 
    : context(context), skillTable(skillTable) {}

    void login(int id);
    void sign_up(int id);
    void set_age(int age);
    void add_skill(const std::string& skill_name);
    void reset_skill();
    void show_age_condition() const;
    void show_skills_condition() const;

    int get_age() const { return age; }
    std::vector<std::string>& get_encoded_skills() { return skills; }
    void send_data_encrpyt(int employee_id, CryptoFactory& factory);
};

#endif //SEAL_REAL_EMPLOYEECLIENT_H
