add_library(index STATIC
    InvertedIndex.cpp
)

target_include_directories(index
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
