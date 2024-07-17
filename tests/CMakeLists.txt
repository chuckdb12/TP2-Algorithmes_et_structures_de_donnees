set(TEST_SOURCES
        testsGrapheTP2.cpp
        testsReseauInterurbrainTP2.cpp
        ${PROJECT_SOURCE_DIR}/Graphe.cpp
        ${PROJECT_SOURCE_DIR}/ReseauInterurbain.cpp)

add_executable(testsAll ${TEST_SOURCES})

target_include_directories(testsAll PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(testsAll
        gtest_main
        gtest
        pthread
)

include(GoogleTest)

gtest_discover_tests(testsAll)
