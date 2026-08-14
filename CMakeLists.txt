cmake_minimum_required(VERSION 3.20)
project(bm25_text_search VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bm25 src/bm25.cpp)
target_include_directories(bm25 PUBLIC include)
target_compile_options(bm25 PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic;-Werror>)

add_executable(bm25-search src/main.cpp)
target_link_libraries(bm25-search PRIVATE bm25)

enable_testing()
add_executable(bm25-tests tests/test_bm25.cpp)
target_link_libraries(bm25-tests PRIVATE bm25)
add_test(NAME bm25-tests COMMAND bm25-tests)
