add_library(query STATIC
    QueryEngine.cpp
)

target_include_directories(query
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(query
    PUBLIC
        parser
        index
)
