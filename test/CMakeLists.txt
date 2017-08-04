
cmake_minimum_required (VERSION 3.8)

add_executable (base64_test main.cpp)

target_link_libraries(base64_test PRIVATE base64 rapidcheck)

