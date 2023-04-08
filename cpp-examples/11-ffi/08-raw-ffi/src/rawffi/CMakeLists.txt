set(target_name rawffi)

add_executable(${target_name} rawffi/main.cpp)

include(CompileOptions)
set_compile_options(${target_name})

find_library(FFI_LIBRARY_PATH ffi)
target_link_libraries(
  ${target_name}
  PRIVATE
    ${FFI_LIBRARY_PATH}
    ${CMAKE_DL_LIBS})
