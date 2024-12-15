//
// Created by jiwoong_dev on 2024-12-06.
//

#include "EmployerClient.h"
#include "../util/util.h"
#include <fstream>
#include "../domain/repository.hpp"
#include "boost/asio.hpp"
#include "../domain/crypto/DomainDecryptor.h"
#include "../domain/crypto/CryptoFactory.h"
#include <filesystem>

void EmployerClient::login(int id) {
    using namespace std;
    const std::string &basicString = to_string(id);
    key_load(publicKey, context, "employer/" +basicString + "_pub");
    key_load(secretKey, context, "employer/" +basicString + "_pri");
    key_load(relinKeys, context, "employer/" +basicString + "_rel");
}

void EmployerClient::sign_up(int id) {
    using namespace std;
    using namespace seal;
    const std::string &basicString = to_string(id);

    KeyGenerator keygen(context);
    keygen.create_public_key(publicKey);
    secretKey = keygen.secret_key();
    keygen.create_relin_keys(relinKeys);

    key_save(publicKey, "employer/" +basicString + "_pub");
    key_save(secretKey, "employer/" +basicString + "_pri");
    key_save(relinKeys, "employer/" +basicString + "_rel");
}

void EmployerClient::set_age(int min, int max) {
    for (int age = min ; age <= max ; age++) {
        this->want.age.push_back(age);
    }
}

void EmployerClient::add_skill(std::string &skill_name) {
    this->want.skill.push_back(skill_name);
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
        std::cout << enc << " ";
    }
    std::cout << "\n";
}

void EmployerClient::query(int id) {
    using namespace std;
    try {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query("localhost", "12345");
        boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        boost::asio::ip::tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        // 밑에서 전송한 값과 구분 가능하게 id 전송
        boost::asio::write(socket, boost::asio::buffer("id:" + to_string(id)));
        // 조건을 서버로 전송
        std::string request = "age:" + std::to_string(want.min_age) + "-" + std::to_string(want.max_age) + ";skills:";
        for (auto& skill : want.skill) {
            int i = skillTable.get_encoded_skill(skill);
            request += i + ",";
        }
        request.pop_back(); // 마지막 쉼표 제거
        boost::asio::write(socket, boost::asio::buffer(request));

        // 서버로부터 응답 수신
        char reply[1024] = {0};
        size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, request.size()));
        std::cout << "Server reply: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
    } 
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

void EmployerClient::query_result(int id) {
    using namespace std;
    string base_path = "result/" + to_string(id) + "/";
    // DomainDecryptor 생성
    // seal_decryptor 생성
    seal::Decryptor seal_decryptor(context, secretKey);
    CryptoFactory factory(context, 4, 3);   
    DomainDecryptor decryptor = factory.createDecryptor(seal_decryptor);

    for (const auto& employer_dir : filesystem::directory_iterator(base_path)) {
        if (employer_dir.is_directory()) {
            string employer_id = employer_dir.path().filename().string();
            cout << "Employer ID: " << employer_id << endl;

            // Load age data
            vector<seal::Ciphertext> age_vector;
            for (size_t i = 0; ; ++i) {
                string age_path = base_path + employer_id + "/age_" + to_string(i);
                if (!filesystem::exists(age_path)) {
                    break;
                }
                seal::Ciphertext age_cipher;
                ifstream age_file(age_path, ios::binary);
                age_cipher.load(context, age_file);
                age_file.close();
                age_vector.push_back(age_cipher);
            }
            int age = decryptor.decrypte_single(age_vector);

            // Load skills data
            string skills_dir = base_path + employer_id + "/skills";
            vector<vector<seal::Ciphertext>> skills_total;
            for (size_t i = 0; ; ++i) {
                vector<seal::Ciphertext> skill_vector;
                for (size_t j = 0; ; ++j) {
                    string skill_path = skills_dir + "/" + to_string(i) + "_" + to_string(j);
                    if (!filesystem::exists(skill_path)) {
                        break;
                    }
                    seal::Ciphertext skill_cipher;
                    ifstream skill_file(skill_path, ios::binary);
                    skill_cipher.load(context, skill_file);
                    skill_file.close();
                    skill_vector.push_back(skill_cipher);
                }
                skills_total.push_back(skill_vector);
            }
            vector<int> skills = decryptor.decrypte_multiple(skills_total);
            cout << "Age: " << age << ", Skills: ";
            for (auto& skill : skills) {
                cout << skill << " ";
            }
            cout << endl;
        }
    }
}