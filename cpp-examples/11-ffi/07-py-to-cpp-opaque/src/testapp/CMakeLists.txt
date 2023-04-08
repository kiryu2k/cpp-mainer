set(target_name testapp)

add_executable(${target_name} testapp/main.c)

include(CompileOptions)
set_compile_options(${target_name})

set_target_properties(
${target_name}
  PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(testapp geometry-c)
