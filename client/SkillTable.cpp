//
// Created by jiwoong_dev on 2024-12-06.
//

#include "SkillTable.h"
#include <iostream>

int SkillTable::get_table_size() {
    return this->skill_encoding_map.size();
}

void SkillTable::showTable(std::ostream& out) {
    int item_index = 0;
    for (int i = 0; i < table.size(); ++i) {
        if (i == 0) continue;
        out << i << "\t";
        for (std::string& name : table[i]) {
            out.width(10);
            out << name << "\t";
        }
        out << "\n\n";
    }
    out << "\n";
}

std::pair<int, int> SkillTable::get_encoded_skill(std::string& skill_name) {
    const std::pair<int, int>& item = this->skill_encoding_map.at(skill_name);
    return std::make_pair(item.first, item.second);
}

std::string SkillTable::get_skill_name(std::pair<size_t, size_t> &enc) {
    for (auto& entry : this->skill_encoding_map) {
        if (entry.second == enc) {
            return entry.first;
        }
    }
    return "wrong";
}