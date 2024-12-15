//
// Created by jiwoongDev on 2024-12-14.
//

#include "checker.h"

seal::Ciphertext Checker::check_age(int min_age, int max_age, std::vector<seal::Ciphertext>& encrypted_age) {
    using namespace std;
    using namespace seal;

    vector<Ciphertext> final_terms;
    for (int age = min_age; age <= max_age ; age++) {
        vector <size_t> encoded_age = encoder.encode(age);
        vector <Ciphertext> terms;
        for (int i = 0 ; i < encrypted_age.size(); i++) {
            size_t c = c_vector[encoded_age[i]];
            Ciphertext match_result  = evaluator.evaluate_matching_circuit(encoded_age[i], c, encrypted_age[i]);
            terms.push_back(match_result);
        }
        Ciphertext final_match = evaluator.evaluate_intersection(terms);
        final_terms.push_back(final_match);
    }
    return evaluator.evaluate_union(final_terms);
}

seal::Ciphertext Checker::check_skills(
    const std::vector<int>& skills, 
    std::vector<std::vector<seal::Ciphertext>>& encrypted_skills
) {
    using namespace std;
    using namespace seal;
    
    vector<Ciphertext> match_results;
    for (int skill: skills) {
        vector <size_t> encoded_skill = encoder.encode(skill);
        vector <Ciphertext> skill_matched;
        for (vector<Ciphertext> &encrypted_skill: encrypted_skills) {

            int enc_width = encrypted_skill.size();
            vector<Ciphertext> full_matched;
            for (int i = 0; i < enc_width; i++) {
                size_t c = c_vector[encoded_skill[i]];
                Ciphertext partial_matched = evaluator.evaluate_matching_circuit(encoded_skill[i], c,encrypted_skill[i]);
                full_matched.push_back(partial_matched);
            }
            Ciphertext match_result = evaluator.evaluate_intersection(full_matched);
            skill_matched.push_back(match_result);
        }
        Ciphertext temp_result = evaluator.evaluate_union(skill_matched);
        cout << "skill" <<  skill << " : ";
        match_results.push_back(temp_result);
    }
    Ciphertext final = evaluator.evaluate_intersection(match_results);
    cout<< "union :";

//    seal::Ciphertext real_final = evaluator.evaluate_matching_circuit(skills.size(), sc, final);
//    evaluator.evaluate_div_inplace(final, sc);
    return final;
}