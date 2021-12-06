include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY  https://github.com/zaphoyd/websocketpp.git
    GIT_TAG         0.8.2
    GIT_SHALLOW     TRUE
)

if(NOT websocketpp_POPULATED)
    FetchContent_Populate(websocketpp)
    add_subdirectory(${websocketpp_SOURCE_DIR} ${websocketpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
