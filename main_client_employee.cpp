//
// Created by jiwoong_dev on 2024-12-06.
//

#include "seal/seal.h"
#include "util/util.h"
#include "client/SkillTable.h"
#include "client/EmployeeClient.h"
#include "domain/repository.hpp"
#include "domain/crypto/CryptoFactory.h"

using namespace std;
using namespace seal;

class EmployeeProgram {
private:
    int phase;
    int id = -1;
    EmployeeClient& client;
    SkillTable& skillTable;
    CryptoFactory& factory;

    int first_phase() {
        int cmd, ipt = -1;
        cout
                << "-------------- menu --------------\n"
                << "1: sign up\n"
                << "2: login\n"
                << "3: exit\n"
                << "----------------------------------\n"
                << "cmd: ";
        cin >> cmd;
        cout << "id: ";
        switch (cmd) {
            case 1:
                cin >> ipt;
                client.sign_up(ipt);
                break;
            case 2:
                cin >> ipt;
                client.login(ipt);
                break;
            default:
                return -1;
        }
        if (ipt == -1) return -1;
        id = ipt;
        phase = 1;
        return cmd;
    }

    bool second_phase() {
        int cmd;
        cout
        << "-------------- menu --------------\n"
        << "1: set age\n"
        << "2: show skills\n"
        << "3: add skill\n"
        << "4: save data\n"
        << "5: show condition\n"
        << "6: log out\n"
        << "----------------------------------\n"
        << "cmd : ";
        cin >> cmd;
        string input_name;
        switch (cmd) {
            case 1:
                int age;
                cout << "Enter min and max age: ";
                cin >> age;
                client.set_age(age);
                break;
            case 2:
                skillTable.showTable(cout);
                break;
            case 3:
                cout << "Input skill name: ";
                cin >> input_name;
                client.add_skill(input_name);
                break;
            case 4:
                client.send_data_encrpyt(id, factory);
                break;
            case 5:
                client.show_age_condition();
                client.show_skills_condition();
                break;
            case 6:
                phase = 0;
                break;
            default:
                return false;
        }
        return true;
    }

public:
    EmployeeProgram(EmployeeClient & client, SkillTable& skillTable, CryptoFactory& factory) 
    : client(client), skillTable(skillTable), factory(factory) {
        this->phase = 0;
        client.reset_skill();
    }
    int show_prompt() {
        switch (phase) {
            case 0:
                return first_phase();
            case 1:
                return second_phase();
        }
        return -1;
    }
};

int main () {
    SkillTable skillTable;
    SEALContext context = load_context();
    CryptoFactory factory(context, 4, 3);
    EmployeeClient client(context, skillTable);
    EmployeeProgram program(client, skillTable, factory);

    int cmd;
    do {
        cmd = program.show_prompt();
        cout << "entered : " << cmd << "\n";
    }while(cmd != -1);

    return 0;
}
