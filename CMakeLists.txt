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
        domain/encoder.h
        domain/encoder.cpp
)

add_executable(client_employer main_client_employer.cpp
        util/util.cpp
        util/util.h
        client/SkillTable.cpp
        client/SkillTable.h
        client/EmployerClient.cpp
        client/EmployerClient.h
        util/TCPClient.hpp
        domain/encoder.h
        domain/encoder.cpp
        domain/decoder.cpp
)

add_executable(client_employee main_client_employee.cpp
        util/util.cpp
        util/util.h
        client/SkillTable.cpp
        client/SkillTable.h
        util/TCPClient.hpp
        domain/respository.hpp
        domain/data.h
        domain/encoder.h
        domain/decoder.h
        domain/encoder.cpp
        domain/decoder.cpp
)

add_executable(test main.cpp
        domain/decoder.cpp
        domain/decoder.h
        domain/encoder.cpp
        domain/encoder.h
)

target_link_libraries(server PRIVATE SEAL::seal)
target_link_libraries(client_employer PRIVATE SEAL::seal)
target_link_libraries(client_employee PRIVATE SEAL::seal)
