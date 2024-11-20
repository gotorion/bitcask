set(TEST_TARGET bitcask_test)

file(GLOB TEST_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)


set(INCLUDE_DIR
        ${CMAKE_SOURCE_DIR}/src/bitcask/fs
        ${CMAKE_SOURCE_DIR}/src/bitcask/common
        ${CMAKE_SOURCE_DIR}/src/bitcask/db
        ${CMAKE_SOURCE_DIR}/src/bitcask/worker
)

set(LINK_DIR
        ${CMAKE_SOURCE_DIR}/out/lib
)

add_executable(${TEST_TARGET} ${TEST_SRCS})
target_include_directories(${TEST_TARGET} PRIVATE ${INCLUDE_DIR})

target_link_libraries(${TEST_TARGET}
        gtest
        gtest_main
        bitcask
        spdlog::spdlog
        absl::status
        absl::statusor
)
