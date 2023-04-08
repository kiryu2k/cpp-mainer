set(target_name grayscale)

add_executable(${target_name}
  grayscale/bitmap/bitmap.hpp
  grayscale/main.cpp)

target_include_directories(
  ${target_name}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/grayscale)

target_link_libraries(
  ${target_name}
  PRIVATE
    cxxopts)

include(CompileOptions)
set_compile_options(${target_name})
