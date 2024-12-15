#include "seal/seal.h"
#include "util/util.h"
#include "domain/repository.hpp"
#include "domain/crypto/DomainEvaluator.h"
#include "domain/repository.hpp"
#include "domain/checker.h"
#include "domain/crypto/CryptoFactory.h"
#include <boost/asio.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace seal;
using boost::asio::ip::tcp;

vector<int> c_vector;

void handle_client(tcp::socket& socket,
                   SEALContext& context,
                   Repository& repository,
                   seal::Evaluator& seal_evaluator) {
    using namespace std;

    try {
        // 클라이언트로부터 조건 수신
        char request[1024];
        size_t request_length = socket.read_some(boost::asio::buffer(request));
        stringstream ss(string(request, request_length));

        // employer_id는 예시로 "employer1"로 설정
        // id 읽기
        char id_request[1024];
        size_t id_length = socket.read_some(boost::asio::buffer(id_request));
        string id_str(id_request, id_length);
        string employer_id = id_str.substr(3); // "id:" 이후의 문자열
        auto employee_data_list = repository.load_all_employee_data(employer_id);

        auto publicKey = repository.load_public_key(employer_id);
        auto relinKey = repository.load_relin_key(employer_id);
        seal::Encryptor seal_encryptor(context, publicKey);
        CryptoFactory factory(context, 4, 3);
        auto encryptor = factory.createEncryptor(seal_encryptor);
        auto evaluator = factory.createEvaluator(seal_evaluator, relinKey, seal_encryptor);
        auto calculator = factory.createCalculator();
        if (c_vector.empty()) {
            c_vector.push_back(1);
            for (int i = 1; i <= 4; i++) {
                c_vector.push_back(calculator.get_numerator_reverse_element(i));
            }
        }
        auto checker = factory.createChecker(evaluator, encryptor, calculator, c_vector);



        // 조건 읽기
        char condition_request[1024];
        size_t condition_length = socket.read_some(boost::asio::buffer(condition_request));
        string condition_str(condition_request, condition_length);

        // 조건 파싱
        string age_condition, skills_condition;
        stringstream condition_ss(condition_str);
        getline(condition_ss, age_condition, ';');
        getline(condition_ss, skills_condition);

        int min_age, max_age;
        sscanf(age_condition.c_str(), "age:%d-%d", &min_age, &max_age);

        vector<int> skill_ids;
        stringstream skills_stream(skills_condition.substr(7)); // "skills:" 이후부터 파싱
        string skill_id;
        while (getline(skills_stream, skill_id, ',')) {
            skill_ids.push_back(stoi(skill_id));
        }

        // 매칭 및 필터링
        vector<EmployeeData> filtered_data;
        for (auto& employee_data : employee_data_list) {
            auto age_match = checker.check_age(min_age, max_age, employee_data.age);
            auto skill_match = checker.check_skills(skill_ids, employee_data.skills);
            Ciphertext match_result;
            seal_evaluator.multiply(age_match, skill_match, match_result);
            seal_evaluator.relinearize_inplace(match_result, relinKey);

            // 각 vector 안의 Ciphertext에 match result 곱하고 relinearize
            for (auto& data : employee_data.skills) {
                for (auto & item : data) {
                    seal_evaluator.multiply_inplace(item, match_result);
                    seal_evaluator.relinearize_inplace(item, relinKey);
                }
            }
            for (auto &item: employee_data.age) {
                seal_evaluator.multiply_inplace(item, match_result);
                seal_evaluator.relinearize_inplace(item, relinKey);
            }
        }
        // 결과 저장
        for (const auto& data : filtered_data) {
            string base_path = "result/" + employer_id + "/" + to_string(data.id) + "/";
            filesystem::create_directories(base_path);

            // age의 각 item 들을 인덱스 번호에 맞게 age_0 age_1 이런식으로 저장
            for (int i = 0; i < data.age.size(); i++) {
                string age_file_name = base_path + "age_" + to_string(i);
                ofstream age_file(age_file_name, ios::binary);
                data.age[i].save(age_file);
                age_file.close();
            }
            // skills 저장 
            for (int i = 0; i < data.skills.size(); i++) {
                string skills_file_name = base_path + "skills_" + to_string(i);
                // 이것도 마찬가지로 인덱스 번호에 맞게 skills_0 skills_1 이런식으로 저장
                for (int j = 0; j < data.skills[i].size(); j++) {
                    string skill_file_name = skills_file_name + "_" + to_string(j);
                    ofstream skill_file(skill_file_name, ios::binary);
                    data.skills[i][j].save(skill_file);
                    skill_file.close();
                }
            }
        }
        boost::asio::write(socket, boost::asio::buffer("done"));
    } catch (exception& e) {
        cerr << "Exception in handle_client: " << e.what() << "\n";
    }
}

int main() {
    try {
        SEALContext context = load_context();
        Repository repository(context);
        seal::Evaluator evaluator(context);
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 12345));

        while (true) {
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            handle_client(socket, context, repository, evaluator);
        }
    } catch (exception& e) {
        cerr << "Exception in main: " << e.what() << "\n";
    }

    return 0;
}
