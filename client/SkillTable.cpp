//
// Created by jiwoong_dev on 2024-12-06.
//

#include "SkillTable.h"
#include <iostream>

int SkillTable::get_table_size() {
    return this->skill_table.size();
}

void SkillTable::showTable(std::ostream& out) {
    int item_index = 0;
    for (int i = 0; i < skill_table.size(); ++i) {
        out.width(13);
        out << i << "." << skill_table[i] << "\t";
        if (i % 5 == 0) {
            out << "\n";
        }
    }
    out << "\n";
}

int SkillTable::get_encoded_skill(std::string& skill_name) {
    for (int i = 0; i < skill_table.size(); ++i) {
        if (skill_table[i] == skill_name) {
            return i;
        }
    }
    return -1;
}

std::string SkillTable::get_skill_name(int& enc) {
    return skill_table[enc];    
}