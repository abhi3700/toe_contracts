cmake_minimum_required(VERSION 3.5)

set(cmake_cxx_standard 17)
set(cmake_cxx_standard_required on)

project(gen_otp)

include_directories(/usr/local/include)

add_executable(gen_otp gen_otp.cpp)
target_link_libraries(gen_otp /usr/local/lib/libhl++.a)