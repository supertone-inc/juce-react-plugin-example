include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
    asio
    GIT_REPOSITORY  https://github.com/chriskohlhoff/asio.git
    GIT_TAG         asio-1-21-0
    GIT_SHALLOW     TRUE
)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
    include_directories(${asio_SOURCE_DIR}/asio/include)
endif()

FetchContent_Declare(
    json
    GIT_REPOSITORY  https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
    GIT_TAG         v3.10.4
    GIT_SHALLOW     TRUE
)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY  https://github.com/juce-framework/JUCE.git
    GIT_TAG         6.1.2
    GIT_SHALLOW     TRUE
)
if(NOT juce_POPULATED)
    FetchContent_Populate(JUCE)
    add_subdirectory(${juce_SOURCE_DIR} ${juce_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY  https://github.com/zaphoyd/websocketpp.git
    GIT_TAG         0.8.2
    GIT_SHALLOW     TRUE
)
if(NOT websocketpp_POPULATED)
    FetchContent_Populate(websocketpp)
    include_directories(${websocketpp_SOURCE_DIR})
endif()