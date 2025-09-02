cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(UIScripter VERSION 1.0.0 LANGUAGES CXX)

add_library(UIScripter INTERFACE)

target_include_directories(UIScripter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(UIScripter INTERFACE cxx_std_20)

CPMAddPackage("gh:camila314/Tiny-fork#master")
CPMAddPackage("gh:camila314/UIBuilder#main")

target_compile_options(${PROJECT_NAME} INTERFACE -g)
target_link_libraries(${PROJECT_NAME} INTERFACE tiny UIBuilder)