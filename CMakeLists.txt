cmake_minimum_required(VERSION 3.15)
project(JUCE_RUST_REACT_PLUGIN_EXAMPLE VERSION 0.0.1)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY  https://github.com/juce-framework/JUCE.git
    GIT_TAG         6.1.2
    GIT_SHALLOW     TRUE
)
FetchContent_MakeAvailable(JUCE)

add_subdirectory(plugin)
