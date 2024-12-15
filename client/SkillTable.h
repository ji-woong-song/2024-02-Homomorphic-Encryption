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
    std::vector<std::string> skill_table = {
        "Django", "FastAPI", "SQLAlchemy", "Pytorch", "Numpy", "Pandas",
        "SpringBoot", "JPA", "QueryDSL", "JUnit", "JSP",
        "React", "Angular", "Vue", "Express", "Next",
        "Git"
    };
public:
    void showTable(std::ostream& out);
    int get_table_size();
    int get_encoded_skill(std::string& skill_name);
    std::string get_skill_name(int& enc);
};


#endif //SEAL_REAL_SKILLTABLE_H
