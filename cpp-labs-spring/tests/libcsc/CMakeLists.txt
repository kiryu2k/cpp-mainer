set(target_name csc_test)

file(GLOB_RECURSE TEST_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(SOURCES ${TEST_SOURCE_FILES})

add_executable(${target_name} ${TEST_SOURCE_FILES})
set_target_properties(
    ${target_name}
    PROPERTIES
    	CXX_STANDARD 20
    	CXX_STANDARD_REQUIRED ON
    	CXX_EXTENSIONS OFF
)
target_link_libraries(${target_name} PRIVATE csc gtest_main)
target_compile_features(${target_name} PUBLIC cxx_std_20)
include(GoogleTest)
gtest_discover_tests(${target_name})