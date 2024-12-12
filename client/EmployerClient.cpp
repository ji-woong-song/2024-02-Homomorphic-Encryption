//
// Created by jiwoong_dev on 2024-12-06.
//

#include "EmployerClient.h"
#include "../util/util.h"
#include <fstream>

void EmployerClient::login(int id) {
    using namespace std;
    const std::string &basicString = to_string(id);
    key_load(publicKey, context, basicString + "_pub");
    key_load(secretKey, context, basicString + "_pri");
    key_load(relinKeys, context, basicString + "_rel");
}

void EmployerClient::sign_up(int id) {
    using namespace std;
    using namespace seal;
    const std::string &basicString = to_string(id);

    KeyGenerator keygen(context);
    keygen.create_public_key(publicKey);
    secretKey = keygen.secret_key();
    keygen.create_relin_keys(relinKeys);

    key_save(publicKey, basicString + "_pub");
    key_save(secretKey, basicString + "_pri");
    key_save(relinKeys, basicString + "_rel");
}

void EmployerClient::set_age(int min, int max) {
    for (int age = min ; age <= max ; age++) {
        this->want.age.push_back(age);
    }
}

void EmployerClient::add_skill(std::string &skill_name) {
    const std::pair<int, int> &encoded_skill = skillTable.get_encoded_skill(skill_name);
    this->want.skill.push_back(std::make_pair(encoded_skill.first, encoded_skill.second));
}

void EmployerClient::reset_skill() {
    this->want.skill.clear();
}

void EmployerClient::show_age_condition() {
    std::cout << "age : " << this->want.min_age << " " << this->want.max_age << "\n";
}

void EmployerClient::show_skills_condition() {
    std::cout << "skill : ";
    for (auto &enc: this->want.skill) {
        std::cout << this->skillTable.get_skill_name(enc) << " ";
    }
    std::cout << "\n";
}

void EmployerClient::query() {

}