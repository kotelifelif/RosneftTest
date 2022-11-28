cmake_minimum_required(VERSION 3.17)
project(test)
set(T test)

set(CMAKE_CXX_STANDARD 17)

add_executable(${T} main.cpp)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${T} PROPERTIES RUNTIME_OUTPUT_DIRECTORY            ${DIR})
set_target_properties(${T} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG      ${DIR})
set_target_properties(${T} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE    ${DIR})
set_target_properties(${T} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY            ${DIR})
set_target_properties(${T} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG      ${DIR})
set_target_properties(${T} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE    ${DIR})
set_target_properties(${T} PROPERTIES LIBRARY_OUTPUT_DIRECTORY            ${DIR})
set_target_properties(${T} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG      ${DIR})
set_target_properties(${T} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE    ${DIR})