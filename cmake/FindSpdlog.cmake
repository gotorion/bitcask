include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY    https://github.com/gabime/spdlog
  GIT_TAG           v1.15.0
)

FetchContent_MakeAvailable(spdlog)
