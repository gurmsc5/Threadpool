file(GLOB test_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(test_threadpool ${test_SRC})
target_link_libraries(test_threadpool INTERFACE threadpool_lib)

enable_testing()

add_test(NAME threadpool_test
        COMMAND test_threadpool
)