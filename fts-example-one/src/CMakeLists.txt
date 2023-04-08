set(target_name indexer)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
    app/indexer.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    cxxopts
    fts
    rapidcsv
)

set(target_name searcher)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
  ${target_name}
  PRIVATE
    app/searcher.cpp
)

target_link_libraries(
  ${target_name}
  PRIVATE
    cxxopts
    fts
    replxx
)

set(target_name fts)

add_library(${target_name} STATIC
  libfts/parser.cpp
  libfts/parser.hpp
  libfts/indexer.cpp
  libfts/indexer.hpp
  libfts/searcher.cpp
  libfts/searcher.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${PROJECT_SOURCE_DIR}/external
)

target_link_libraries(
  ${target_name}
  PRIVATE
   nlohmann_json
   picosha2
   fmt
)

set(target_name fts-c)

add_library(${target_name} SHARED
  libfts-c/parser.cpp 
  libfts-c/parser.h
  libfts-c/searcher.cpp 
  libfts-c/searcher.h)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(
  ${target_name}
  PRIVATE
   fts
)

add_custom_command(TARGET ${target_name} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   $<TARGET_FILE:${target_name}> ${PROJECT_SOURCE_DIR}/client)

set(target_name test_fts)

add_executable(${target_name} libfts/parser.test.cpp libfts/indexer.test.cpp libfts/searcher.test.cpp)

target_link_libraries(
  ${target_name}
  PRIVATE
    gtest_main
    fts
)

include(GoogleTest)

gtest_discover_tests(${target_name})