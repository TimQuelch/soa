find_package(benchmark)

add_executable(soa-benchmark soa-benchmark.cpp)
target_link_libraries(soa-benchmark soa benchmark)

add_custom_target(run-benchmark COMMAND ${CMAKE_CURRENT_BINARY_DIR}/soa-benchmark)
