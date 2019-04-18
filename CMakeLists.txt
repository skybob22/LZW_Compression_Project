cmake_minimum_required(VERSION 3.13)
project(LZW_Compression)

set(CMAKE_CXX_STANDARD 17)


set(SOURCE_FILES main.cpp)
add_executable(LZW ${SOURCE_FILES})


include_directories(Source_Files/Header)



add_subdirectory(Source_Files LZW_Source)
add_subdirectory(Test_Files)
target_link_libraries(LZW LZW_Source)