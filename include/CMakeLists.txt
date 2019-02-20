#message("${PARENT_DIR}/include/threadpool.h")

file(GLOB INCLUDE_SRC ${PARENT_DIR}/include/*.hpp)

add_library(threadpool_lib INTERFACE)
target_include_directories(threadpool_lib INTERFACE ${INCLUDE_SRC})

if(UNIX)
    target_link_libraries(threadpool_lib pthread)
endif()