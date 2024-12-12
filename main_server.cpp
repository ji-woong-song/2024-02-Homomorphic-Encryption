#include "seal/seal.h"
#include "util/util.h"
#include <iostream>
#include <fstream>
#include "domain/matcher.hpp"
#include <vector>
#include "domain/MatchingService.hpp"

using namespace std;
using namespace seal;


void init_c_table(const char * filename, vector<size_t>& c_vector) {
    // c_table 파일에서 데이터 읽기
    ifstream file_input("c_table", ifstream::in);
    if (!file_input.is_open()) {
        cout << "파일 연결 실패" << "\n";
    }

    string line;
    file_input >> line;
    file_input >> line;
    file_input >> line;

    c_vector.push_back(1);
    for (int i = 0; i < 10 ; i++) {
        size_t item = 0;
        file_input >> line;
        for (auto & c : line) {
            item = 10 * item + (c - '0');
        }
        cout << item << "\n";
        c_vector.push_back(item); // 각 줄을 정수로 변환하여 c_vector에 추가
    }
    file_input.close(); // 파일 닫기
}


int main() {
    size_t poly_modulus_degree = 16384;
    size_t plain_modulus_value = 362897;
    SEALContext context = load_context();

    vector<size_t> c_vector;
    init_c_table("c_table", c_vector);

    KeyGenerator keygen(context);
    PublicKey public_key;
    keygen.create_public_key(public_key);
    SecretKey secret_key = keygen.secret_key();
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);

    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    Matcher matcher(10, c_vector, plain_modulus_value);
    Repository repository(context);
    Monitor monitor(decryptor, plain_modulus_value);
    MatchingService matchingService(monitor, repository, matcher, context);


    vector<pair<size_t, size_t>> user_ages = {
            {2, 1},
            {2,5}
    };
    vector<vector<pair<size_t,size_t>>> user_skills = {
        {{1 , 1}, {1, 2}, {1, 3}, {2, 1}},
        {{1 , 2}, {1, 3}, {2, 1}, {2, 2}},
    };
    
    vector<EmployeeData> encrypted_data;
    for (int i = 0 ; i < user_ages.size() ; i++) {
        auto &age = user_ages[i];
        auto &skills = user_skills[i];

        EmployeeData data;
        data.id = i;
        Ciphertext a_first =  encrypt(age.first, encryptor);
        Ciphertext b_first = encrypt(age.second, encryptor);
        data.age = {a_first, b_first};
        vector<pair<Ciphertext, Ciphertext>> cipher_skills;
        for (auto & skill : skills) {
            seal::Ciphertext first = encrypt(skill.first, encryptor);
            seal::Ciphertext second =encrypt(skill.second, encryptor);
            cipher_skills.push_back({first, second});
        }
        data.skills = cipher_skills;
        encrypted_data.push_back(data);
    }

    vector<pair<size_t, size_t>> ages = {{2, 1}, {2,2} ,{2, 3}, {2, 4}};
    vector<pair<size_t, size_t>> skills = {{1, 1}, {2, 1}};

    matchingService.filter(encrypted_data, public_key, relin_keys, ages, skills);

    // decrypt
    cout << "\n=== 원본 데이터 및 복호화 결과 ===" << endl;
    
    for (const auto& data : encrypted_data) {
        cout << "\n지원자 ID: " << data.id << endl;
        
        // 나이 복호화
        Plaintext age_plain1, age_plain2;
        decryptor.decrypt(data.age.first, age_plain1);
        decryptor.decrypt(data.age.second, age_plain2);
        cout << "나이: (" << age_plain1.data()[0] << ", " << age_plain2.data()[0] << ")" << endl;
        
        // 스킬 복호화
        cout << "스킬: ";
        for (const auto& skill : data.skills) {
            Plaintext skill_plain1, skill_plain2;
            decryptor.decrypt(skill.first, skill_plain1);
            decryptor.decrypt(skill.second, skill_plain2);
            cout << "(" << skill_plain1.data()[0] << ", " << skill_plain2.data()[0] << ") ";
        }
        cout << endl;
    }

    return 0;
}
