include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
    json
    GIT_REPOSITORY  https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
    GIT_TAG         v3.10.4
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(json)
