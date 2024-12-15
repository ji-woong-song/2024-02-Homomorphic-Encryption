//
// Created by jiwoong_dev on 2024-12-14.
//

#include "domain/crypto/CryptoFactory.h"
#include "util/util.h"

using namespace seal;
using namespace std;

int main() {
    SEALContext context = load_context();
    size_t ploy_modulus = context.first_context_data()->parms().plain_modulus().value();
    KeyGenerator keyGen(context);

    SecretKey secretKey = keyGen.secret_key();
    PublicKey publicKey;
    keyGen.create_public_key(publicKey);
    RelinKeys relinKeys;
    keyGen.create_relin_keys(relinKeys);

    Encryptor encryptor_s(context, publicKey);
    Decryptor decryptor_s(context, secretKey);
    Evaluator evaluator_s(context);

    CryptoFactory factory(context, 4, 3);
    DomainEncryptor encryptor = factory.createEncryptor(encryptor_s);
    DomainDecryptor decryptor = factory.createDecryptor(decryptor_s);
    DomainEvaluator evaluator = factory.createEvaluator(evaluator_s, relinKeys,  encryptor_s);
    Calculator calculator = factory.createCalculator();
    Monitor monitor(decryptor_s, ploy_modulus);
    vector<int> c_vector;
    c_vector.push_back(1);
    for (int i = 1; i <= 4; i++) {
        c_vector.push_back(calculator.get_numerator_reverse_element(i));
    }
    Checker checker = factory.createChecker(evaluator, encryptor, calculator, c_vector);

//    vector<seal::Ciphertext> encrypteSingle = encryptor.encrypte_single(3);
    vector<int> skills {10, 11, 13};
    vector<int> require{10, 11, 12, 13, 14, 15, 16, 17, 18};
    
    vector<std::vector<seal::Ciphertext>> multiple = encryptor.encrypte_multiple(skills);
    Ciphertext result = checker.check_skills(require, multiple);
    Plaintext plaintext;
    decryptor_s.decrypt(result, plaintext);
    cout << plaintext.data()[0] << endl;
    return 0;
}