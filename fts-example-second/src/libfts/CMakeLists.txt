set(target_name fts)

add_library(${target_name} STATIC
  parser.cpp
  indexer.cpp
  searcher.cpp
  ${PROJECT_SOURCE_DIR}/include/libfts/parser.hpp
  ${PROJECT_SOURCE_DIR}/include/libfts/indexer.hpp
  ${PROJECT_SOURCE_DIR}/include/libfts/searcher.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(
  ${target_name}
  PRIVATE
    nlohmann_json
    picosha2
)

# Now simply link against gtest or gtest_main as needed. Eg

set(target_name fts_test)

add_executable(${target_name} parser.test.cpp indexer.test.cpp searcher.test.cpp)

target_link_libraries(
  ${target_name}
  PRIVATE
    fts
    gtest_main
)

include(GoogleTest)

gtest_discover_tests(${target_name})