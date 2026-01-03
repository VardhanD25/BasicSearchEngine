add_library(indexer STATIC
    Indexer.cpp
    IndexWriter.cpp
    IndexReader.cpp
)

target_include_directories(indexer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(indexer
    PUBLIC
        parser
        index
)
