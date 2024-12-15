//
// Created by jiwoong_dev on 2024-12-06.
//

#ifndef SEAL_REAL_EMPLOYERCLIENT_H
#define SEAL_REAL_EMPLOYERCLIENT_H

#include "seal/seal.h"
#include "SkillTable.h"
#include "../util/util.h"

struct EmployerWant {
    int min_age;
    int max_age;
    std::vector<size_t> age;
    std::vector<std::string> skill;
};

class EmployerClient {
private:
    seal::PublicKey publicKey;
    seal::SecretKey secretKey;
    seal::RelinKeys relinKeys;
    seal::SEALContext& context;
    SkillTable& skillTable;
    EmployerWant want;
public:
    EmployerClient(seal::SEALContext & context, SkillTable& skillTable) : context(context), skillTable(skillTable) {}
    /*
     * load keys from local storage
     * */
    void login(int id);
    /*
     * generate keys send pubkey, relinkey to server
     * save keys in local storage
     * */
    void sign_up(int id);

    void set_age(int min, int max);
    void add_skill(std::string& skill_name);
    void reset_skill();
    void show_age_condition();
    void show_skills_condition();
    void query(int id);

    // Public key와 Relin key를 저장하는 함수
    void save_keys(const std::string& employer_id);

    void query_result(int id);
};


#endif //SEAL_REAL_EMPLOYERCLIENT_H
