cmake_minimum_required(VERSION 3.5)

set(cmake_cxx_standard 17)
set(cmake_cxx_standard_required on)

project(profile_hash)

include_directories(/usr/local/include)

add_executable(profile_hash main.cpp)
target_link_libraries(profile_hash /usr/local/lib/libhl++.a)