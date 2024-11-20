set(TARGET_NAME bitcask)

file(GLOB SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/bitcask/fs/*.cc
)

set(PUB_INCLUDE_DIR
        ${CMAKE_SOURCE_DIR}/include
)

set(PRIVATE_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/bitcask/common
        ${CMAKE_CURRENT_SOURCE_DIR}/bitcask/fs
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

add_library(${TARGET_NAME} ${SRC_FILES})

target_include_directories(
        ${TARGET_NAME}
        AFTER PRIVATE
        ${PRIVATE_INCLUDE_DIR}
        BEFORE PUBLIC
        ${PUB_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME}
        PRIVATE
        spdlog::spdlog
        absl::status
        absl::statusor
)
