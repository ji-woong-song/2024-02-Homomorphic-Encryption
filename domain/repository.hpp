//
// Created by jiwoong_dev on 2024-12-08.
//

#ifndef SEAL_REAL_REPOSITORY_HPP
#define SEAL_REAL_REPOSITORY_HPP

#include "data.h"
#include "../util/util.h"
#include <fstream> // 추가된 헤더
#include <string>  // 추가된 헤더
#include <filesystem>
#include <vector>  // 추가된 헤더
#include "seal/seal.h" // Seal 라이브러리 포함

class Repository {
private:
    seal::SEALContext& context;
    const std::string pub_base_path =  "data/employer/pub/" ;
    const std::string relin_base_path =  "data/employer/relin/";
    const std::string employee_data_path = "data/employee/";
public:
    Repository(seal::SEALContext& context) : context(context) {}

    void save_pub_key(const std::string& id, seal::PublicKey& pub, seal::RelinKeys& relin) {
        std::string pub_path = pub_base_path + id;
        std::string relin_path = relin_base_path + id;
        key_save(pub, pub_path);
        key_save(relin, relin_path);
    }

    std::vector<PublicKeyEntry> load_all_pub_keys() {
        std::vector<PublicKeyEntry> pub_keys;
        
        for (const auto& entry : std::filesystem::directory_iterator(pub_base_path)) {
            if (entry.is_regular_file()) {

                seal::PublicKey pub_key;
                key_load(pub_key, context, entry.path().string());
                std::string id = entry.path().filename().string();
                pub_keys.push_back({id, pub_key});
            }
        }
        return pub_keys;
    }

    void save_employee_data(const std::string& employee_id, const std::string& employer_id, EmployeeData& data) {
        std::string dir_path = employee_data_path + employer_id + "/" + employee_id;
        std::filesystem::create_directories(dir_path);

        // Save age vector
        for (size_t i = 0; i < data.age.size(); ++i) {
            std::string age_path = dir_path + "/age_" + std::to_string(i);
            cipher_save(data.age[i], age_path);
        }

        // Save skills vectors
        if (!std::filesystem::exists(dir_path + "/skills")) {
            std::filesystem::create_directories(dir_path + "/skills");
        }
        for (size_t i = 0; i < data.skills.size(); ++i) {
            for (size_t j = 0; j < data.skills[i].size(); ++j) {
                std::string skill_path = dir_path + "/skills/" + std::to_string(i) + "_" + std::to_string(j);
                cipher_save(data.skills[i][j], skill_path);
            }
        }
    }

    std::vector<EmployeeData> load_all_employee_data(const std::string& employer_id) {
        std::vector<EmployeeData> all_employee_data;
        std::string dir_path = employee_data_path + employer_id;

        for (const auto& employee_dir : std::filesystem::directory_iterator(dir_path)) {
            if (employee_dir.is_directory()) {
                EmployeeData employee_data;
                std::string employee_id = employee_dir.path().filename().string();
                employee_data.id = std::stoi(employee_id);

                // Load age vector
                for (size_t i = 0; ; ++i) {
                    std::string age_path = dir_path + "/" + employee_id + "/age_" + std::to_string(i);
                    if (!std::filesystem::exists(age_path)) {
                        break;
                    }
                    seal::Ciphertext age_cipher;
                    cipher_load(age_cipher, context, age_path);
                    employee_data.age.push_back(age_cipher);
                }

                // Load skills vectors
                std::string skills_dir = dir_path + "/" + employee_id + "/skills";
                for (size_t i = 0; ; ++i) {
                    std::vector<seal::Ciphertext> skill_vector;
                    for (size_t j = 0; ; ++j) {
                        std::string skill_path = skills_dir + "/" + std::to_string(i) + "_" + std::to_string(j);
                        if (!std::filesystem::exists(skill_path)) {
                            break;
                        }
                        seal::Ciphertext skill_cipher;
                        cipher_load(skill_cipher, context, skill_path);
                        skill_vector.push_back(skill_cipher);
                    }
                    if (skill_vector.empty()) {
                        break;
                    }
                    employee_data.skills.push_back(skill_vector);
                }
                all_employee_data.push_back(employee_data);
            }
        }
        return all_employee_data;
    }

    seal::RelinKeys load_relin_key(const std::string& employer_id) {
        std::string relin_path = relin_base_path + employer_id;
        seal::RelinKeys relin_keys;
        key_load(relin_keys, context, relin_path);
        return relin_keys;
    }

    seal::PublicKey load_public_key(const std::string& employer_id) {
        std::string pub_path = pub_base_path + employer_id;
        seal::PublicKey pub_key;
        key_load(pub_key, context, pub_path);
        return pub_key;
    }
};

#endif //SEAL_REAL_REPOSITORY_HPP
