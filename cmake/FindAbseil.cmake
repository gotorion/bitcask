include(FetchContent)

FetchContent_Declare(
        abseilcpp
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG 20240722.0
)

set(ABSL_PROPAGATE_CXX_STD ON)

FetchContent_MakeAvailable(abseilcpp)
