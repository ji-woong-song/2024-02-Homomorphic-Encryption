//
// Created by jiwoong_dev on 2024-12-06.
//

#ifndef SEAL_REAL_SKILLTABLE_H
#define SEAL_REAL_SKILLTABLE_H

#include <iostream>
#include <vector>
#include <map>

class SkillTable {
private:
    std::map<std::string, std::pair<size_t, size_t>> skill_encoding_map = {
        {"Django", std::make_pair<size_t, size_t>(1, 1)},
        {"FastAPI", std::make_pair<size_t, size_t>(1, 2)},
        {"SQLAlchemy", std::make_pair<size_t, size_t>(1, 3)},
        {"Pytorch", std::make_pair<size_t, size_t>(1, 4)},
        {"Numpy", std::make_pair<size_t, size_t>(1, 5)},
        {"Pandas", std::make_pair<size_t, size_t>(1, 6)},

        {"SpringBoot", std::make_pair<size_t, size_t>(2, 1)},
        {"JPA", std::make_pair<size_t, size_t>(2, 2)},
        {"QueryDSL", std::make_pair<size_t, size_t>(2, 3)},
        {"JUnit", std::make_pair<size_t, size_t>(2, 4)},
        {"JSP", std::make_pair<size_t, size_t>(2, 5)},

        {"React", std::make_pair<size_t, size_t>(3, 1)},
        {"Angular", std::make_pair<size_t, size_t>(3, 2)},
        {"Vue", std::make_pair<size_t, size_t>(3, 3)},
        {"Express", std::make_pair<size_t, size_t>(3, 4)},
        {"Next", std::make_pair<size_t, size_t>(3, 5)},

        {"Git", std::make_pair<size_t, size_t>(4, 1)},
    };
    std::vector<std::vector<std::string>> table;
public:
    SkillTable() {
        table.assign(5, std::vector<std::string>());
        for (auto & entry : skill_encoding_map) {
            table[entry.second.first].push_back(entry.first);
        }
    }
    void showTable(std::ostream& out);
    int get_table_size();
    std::pair<int, int> get_encoded_skill(std::string& skill_name);
    std::string get_skill_name(std::pair<size_t, size_t>& enc);
};


#endif //SEAL_REAL_SKILLTABLE_H
