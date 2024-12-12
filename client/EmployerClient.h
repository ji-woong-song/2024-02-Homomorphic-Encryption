//
// Created by jiwoong_dev on 2024-12-06.
//

#ifndef SEAL_REAL_EMPLOYERCLIENT_H
#define SEAL_REAL_EMPLOYERCLIENT_H

#include "seal/seal.h"
#include "SkillTable.h"

struct EmployerWant {
    int min_age;
    int max_age;
    std::vector<size_t> age;
    std::vector<std::pair<size_t, size_t>> skill;
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
    /*
     * update server data with local state
     * */
    void query();
    void sync_key();
};


#endif //SEAL_REAL_EMPLOYERCLIENT_H
