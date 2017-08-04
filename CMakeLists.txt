
cmake_minimum_required (VERSION 3.8)

project (BASE64 VERSION 1.0.0 LANGUAGES CXX)

list (INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/ext)

enable_testing ()

add_library (base64 INTERFACE)
target_include_directories(base64 INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

add_subdirectory (ext/rapidcheck)
add_subdirectory (test)
