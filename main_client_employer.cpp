//
// Created by jiwoong_dev on 2024-12-06.
//
#include "seal/seal.h"
#include "util/util.h"
#include "client/SkillTable.h"
#include "client/EmployerClient.h"

using namespace std;
using namespace seal;

class EmployerProgram {
private:
    int phase;
    int id = -1;
    EmployerClient& client;
    SkillTable& skillTable;

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
        << "4: query\n"
        << "5: show condition\n"
        << "6: log out\n"
        << "7: view query results\n"
        << "----------------------------------\n"
        << "cmd : ";
        cin >> cmd;
        string input_name;
        switch (cmd) {
            case 1:
                int min_age, max_age;
                cout << "Enter min and max age: ";
                cin >> min_age >> max_age;
                client.set_age(min_age, max_age);
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
                client.query(id);
                break;
            case 5:
                client.show_age_condition();
                client.show_skills_condition();
                break;
            case 6:
                phase = 0;
                break;
            case 7:
                client.query_result(id);
                break;
            default:
                return false;
        }
        return true;
    }

public:
    EmployerProgram(EmployerClient & client, SkillTable& skillTable) : client(client), skillTable(skillTable) {
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
    skillTable.showTable(cout);
    SEALContext context = load_context();
    EmployerClient client(context, skillTable);
    EmployerProgram program(client, skillTable);

    int cmd;
    do {
        cmd = program.show_prompt();
        cout << "entered : " << cmd << "\n";
    }while(cmd != -1);

    return 0;
}