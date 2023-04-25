include(FetchContent)

FetchContent_Declare(
    rapidcsv
    GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
    GIT_TAG v8.75
    GIT_SHALLOW TRUE
    PREFIX ${CMAKE_CURRENT_BINARY_DIR})

FetchContent_MakeAvailable(rapidcsv)