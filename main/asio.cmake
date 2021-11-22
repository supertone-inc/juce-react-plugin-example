include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
    asio
    GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
    GIT_TAG         asio-1-21-0
    GIT_SHALLOW     TRUE
)

FetchContent_MakeAvailable(asio)
