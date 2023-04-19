set(target_name csc)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/absolutepath.hpp.in absolutepath.hpp)

# file(GLOB_RECURSE LIBCSC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

file(GLOB_RECURSE LIBCSC_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
# add_library(${target_name} STATIC ${LIBCSC_SOURCE_FILES} ${LIBCSC_HEADER_FILES})
add_library(${target_name} INTERFACE ${LIBCSC_SOURCE_FILES})
# set_compile_options(${target_name})
target_include_directories(${target_name} INTERFACE ${PROJECT_SOURCE_DIR}/src/libcsc)