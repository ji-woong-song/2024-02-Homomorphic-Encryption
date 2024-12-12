//
// Created by jiwoong_dev on 2024-12-08.
//

#ifndef SEAL_REAL_RESPOSITORY_HPP
#define SEAL_REAL_RESPOSITORY_HPP

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

    void save_employee_data(std::string& employee_id, std::string& employer_id, EmployeeData& data) {
        std::string dir_path = employee_data_path + employer_id + "/" + employee_id;
        std::filesystem::create_directories(dir_path);

        // Save age pair
        std::string age_path_1 = dir_path + "/age_1";
        cipher_save(data.age.first, age_path_1);
        std::string age_path_2 = dir_path + "/age_2";
        cipher_save(data.age.second, age_path_2);

        // Save skills pairs
        if (!std::filesystem::exists(dir_path + "/skills")) {
            std::filesystem::create_directories(dir_path + "/skills");
        }
        int index = 0;
        for (auto &skill : data.skills) {
            std::string skill_path_1 = dir_path + "/skills/" + std::to_string(index) + "_1";
            cipher_save(skill.first, skill_path_1);
            std::string skill_path_2 = dir_path + "/skills/" + std::to_string(index) + "_2";
            cipher_save(skill.second, skill_path_2);
            index++;
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

                // Load age pair
                std::string age_path_1 = dir_path + "/" + employee_id + "/age_1";
                cipher_load(employee_data.age.first, context, age_path_1);
                std::string age_path_2 = dir_path + "/" + employee_id + "/age_2";
                cipher_load(employee_data.age.second, context, age_path_2);

                // Load skills pairs
                std::string skills_dir = dir_path + "/" + employee_id + "/skills";
                for (int index = 0; ; ++index) {
                    std::string skill_path_1 = skills_dir + "/" + std::to_string(index) + "_1";
                    std::string skill_path_2 = skills_dir + "/" + std::to_string(index) + "_2";
                    if (!std::filesystem::exists(skill_path_1) || !std::filesystem::exists(skill_path_2)) {
                        break;
                    }
                    seal::Ciphertext skill_first, skill_second;
                    cipher_load(skill_first, context, skill_path_1);
                    cipher_load(skill_second, context, skill_path_2);
                    employee_data.skills.push_back({skill_first, skill_second});
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

#endif //SEAL_REAL_RESPOSITORY_HPP
