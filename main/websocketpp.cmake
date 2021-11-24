include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY  https://github.com/zaphoyd/websocketpp.git
    GIT_TAG         0.8.2
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(websocketpp)
