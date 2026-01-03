add_executable(search_engine
    main.cpp
)

add_subdirectory(utils)
add_subdirectory(parser)
add_subdirectory(index)
add_subdirectory(query)
add_subdirectory(indexer)

target_link_libraries(search_engine
    PRIVATE
        utils
        parser
        index
        query
        indexer
)