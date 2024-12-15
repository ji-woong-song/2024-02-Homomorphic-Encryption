//
// Created by jiwoong_dev on 2024-12-15.
//
#include "seal/seal.h"
#include "../util/util.h"
#include "../client/SkillTable.h"
#include "../domain/repository.hpp"
#include "EmployeeClient.h"

using namespace std;
using namespace seal;

void EmployeeClient::login(int id) {
    using namespace std;
    const std::string &basicString = to_string(id);
    key_load(publicKey, context, "employee/" + basicString + "_pub");
    key_load(secretKey, context, "employee/" + basicString + "_pri");
    key_load(relinKeys, context, "employee/" + basicString + "_rel");
}

void EmployeeClient::sign_up(int id) {
    using namespace std;
    using namespace seal;
    const std::string &basicString = to_string(id);

    KeyGenerator keygen(context);
    keygen.create_public_key(publicKey);
    secretKey = keygen.secret_key();
    keygen.create_relin_keys(relinKeys);

    key_save(publicKey, "employee/" + basicString + "_pub");
    key_save(secretKey, "employee/" + basicString + "_pri");
    key_save(relinKeys, "employee/" + basicString + "_rel");
}

void EmployeeClient::set_age(int age) {
    this->age = age;
}

void EmployeeClient::add_skill(const std::string &skill_name) {
    this->skills.push_back(skill_name);
}

void EmployeeClient::reset_skill() {
    skills.clear();
}

void EmployeeClient::show_age_condition() const {
    std::cout << "age: " << age << "\n";
}

void EmployeeClient::show_skills_condition() const {
    std::cout << "Encoded skills: ";
    for (const auto &skill: skills) {
        std::cout << skill << " ";
    }
    std::cout << "\n";
}


/*
    repository에서 모든 employer의 공개키를 가져오고
    가져온 공개키로 암호화하여 EmployerData 생성
    repository에 저장
 */
void EmployeeClient::send_data_encrpyt(int employee_id, CryptoFactory &factory) {
    using namespace seal;
    using namespace std;

    Repository repository(context);
    auto employer_keys = repository.load_all_pub_keys();
    vector<int> skill_nums;
    for (auto &skill: skills) {
        skill_nums.push_back(skillTable.get_encoded_skill(skill));
    }

    for (const auto &item: employer_keys) {
        Encryptor encryptor(context, item.pub_key);
        DomainEncryptor domain_encryptor = factory.createEncryptor(encryptor);
        auto encrypted_age = domain_encryptor.encrypte_single(age);
        auto encrypted_skills = domain_encryptor.encrypte_multiple(skill_nums);
        EmployeeData data;
        data.id = employee_id;
        data.age = encrypted_age;
        data.skills = encrypted_skills;
        std::string emp_id = std::to_string(employee_id);
        repository.save_employee_data(emp_id, item.id, data);
    }
}
