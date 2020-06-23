project(demo)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( demo demo demo.cpp )
target_include_directories( demo PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( demo ${CMAKE_SOURCE_DIR}/../ricardian )