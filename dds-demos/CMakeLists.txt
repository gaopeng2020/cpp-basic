project(dds_demo VERSION 1.0.0 LANGUAGES CXX)

find_package(fastdds CONFIG REQUIRED)
find_package(fastcdr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(01_hello_world)
add_subdirectory(02_static_edp_discovery)
add_subdirectory(03_dynamic_pdp_discovery)
add_subdirectory(04_delivery_mechanisms)
add_subdirectory(05_xtypes)



