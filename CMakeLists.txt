cmake_minimum_required(VERSION 3.27)
project(seal_real)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "C:/Users/jiwoong_dev/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(SEAL CONFIG REQUIRED)
#find_package(Boost REQUIRED)

add_executable(server main_server.cpp
        util/util.cpp
        util/util.h
        domain/matcher.hpp
        domain/Monitor.hpp
        domain/respository.hpp
        domain/MatchingService.hpp
        domain/transform/encoder.h
        domain/transform/encoder.cpp
        domain/crypto/DomainEncryptor.cpp
        domain/crypto/DomainEncryptor.h
        domain/crypto/CryptoFactory.h
        domain/calculator/calculator.cpp
        domain/calculator/calculator.h
        domain/crypto/DomainEvaluator.cpp
        domain/crypto/DomainEvaluator.h
)

add_executable(client_employer main_client_employer.cpp
        util/util.cpp
        util/util.h
        client/SkillTable.cpp
        client/SkillTable.h
        client/EmployerClient.cpp
        client/EmployerClient.h
        util/TCPClient.hpp
        domain/transform/encoder.h
        domain/transform/encoder.cpp
        domain/transform/decoder.cpp
        domain/crypto/CryptoFactory.h
        domain/crypto/DomainDecryptor.h
        domain/calculator/calculator.cpp
        domain/calculator/calculator.h
)

add_executable(client_employee main_client_employee.cpp
        util/util.cpp
        util/util.h
        client/SkillTable.cpp
        client/SkillTable.h
        util/TCPClient.hpp
        domain/respository.hpp
        domain/data.h
        domain/transform/encoder.h
        domain/transform/decoder.h
        domain/transform/encoder.cpp
        domain/transform/decoder.cpp
        domain/crypto/DomainEncryptor.cpp
        domain/crypto/DomainEncryptor.h
        domain/crypto/CryptoFactory.cpp
        domain/crypto/CryptoFactory.h
        domain/crypto/DomainDecryptor.cpp
        domain/crypto/DomainDecryptor.h
        domain/checker.cpp
        domain/checker.h
        domain/calculator/calculator.cpp
        domain/calculator/calculator.h
)

add_executable(test main.cpp
        domain/transform/decoder.cpp
        domain/transform/decoder.h
        domain/transform/encoder.cpp
        domain/transform/encoder.h
        domain/crypto/DomainDecryptor.cpp
        domain/crypto/DomainDecryptor.h
        domain/crypto/DomainEncryptor.cpp
        domain/crypto/DomainEncryptor.h
        domain/crypto/CryptoFactory.cpp
        domain/crypto/CryptoFactory.h
        util/util.cpp
        util/util.h
        domain/calculator/calculator.cpp
        domain/calculator/calculator.h
        domain/crypto/DomainEvaluator.cpp
        domain/crypto/DomainEvaluator.h
        domain/checker.cpp
        domain/checker.h
)

target_link_libraries(server PRIVATE SEAL::seal)
target_link_libraries(client_employer PRIVATE SEAL::seal)
target_link_libraries(client_employee PRIVATE SEAL::seal)
target_link_libraries(test PRIVATE SEAL::seal)