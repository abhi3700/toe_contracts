project(cabeosride)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( cabeosride cabeosride cabeosride.cpp )
target_include_directories( cabeosride PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( cabeosride ${CMAKE_SOURCE_DIR}/../ricardian )