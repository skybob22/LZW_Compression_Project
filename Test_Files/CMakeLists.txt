project(LZW_Tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests
        HashTableTests.cpp
        EncodeDictionaryTests.cpp
        DecodeDictionaryTests.cpp
        combinedDictonaryTests.cpp
        LZWCodecTests.cpp)

add_subdirectory(gTest)

target_link_libraries(runTests gtest gtest_main)
target_link_libraries(runTests LZW_Source)