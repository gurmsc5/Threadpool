file(GLOB INCLUDE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_library(threadpool_lib STATIC ${INCLUDE_SRC})
set_target_properties(threadpool_lib PROPERTIES LINKER_LANGUAGE CXX)

if(UNIX)
    target_link_libraries(threadpool_lib pthread)
endif()