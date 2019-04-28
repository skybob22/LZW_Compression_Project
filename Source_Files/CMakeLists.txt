set(HEADER_FILES
        Header/HashTable.h
        Header/EncodeDictionary.h
        Header/DecodeDictionary.h
        Header/LZWCodec.h)
set(SOURCE_FILES
        ../main.cpp
        Source/LZWCodec.cpp
        Source/UtilityFunctions.cpp)

include_directories(Header)
include_directories(Source)

add_library(LZW_Source STATIC ${SOURCE_FILES} ${HEADER_FILES})