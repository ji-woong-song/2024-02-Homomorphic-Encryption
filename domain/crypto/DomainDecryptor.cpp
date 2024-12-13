//
// Created by jiwoongDev on 2024-12-13.
//

#include "DomainDecryptor.h"

int DomainDecryptor::decrypteAge(const seal::Ciphertext &age) {
    using namespace std;
    using namespace seal;
    Plaintext plaintext;
    decryptor.decrypt(age, plaintext);
    BatchEncoder batchEncoder(context);
    vector<size_t> vec;
    batchEncoder.decode(plaintext, vec);
    int result = decoder.decode(vec);
    return result;
}

std::vector<int> DomainDecryptor::decrypteSkills(const seal::Ciphertext &skill) {
    using namespace std;
    using namespace seal;
    Plaintext plaintext;
    decryptor.decrypt(skill, plaintext);
    BatchEncoder batchEncoder(context);
    vector<size_t> vec;
    batchEncoder.decode(plaintext, vec);
    std::vector<int> result = decoder.decode_batch(vec);
    return result;
}