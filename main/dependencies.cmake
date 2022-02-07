include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

set(BOOST_SUBMODULES
    libs/asio
    libs/assert
    libs/bind
    libs/config
    libs/core
    libs/date_time
    libs/hana
    libs/intrusive
    libs/move
    libs/mpl
    libs/numeric/conversion
    libs/predef
    libs/preprocessor
    libs/regex
    libs/smart_ptr
    libs/static_assert
    libs/system
    libs/throw_exception
    libs/utility
    libs/type_traits
)

if(WIN32)
    list(APPEND BOOST_SUBMODULES
        libs/container_hash
        libs/detail
        libs/integer
        libs/type_index
        libs/winapi
        libs/variant
    )
endif()

FetchContent_Declare(
    boost
    GIT_REPOSITORY  https://github.com/boostorg/boost.git
    GIT_TAG         boost-1.78.0
    GIT_SHALLOW     TRUE
    GIT_SUBMODULES  ${BOOST_SUBMODULES}
)
FetchContent_Populate(boost)
foreach(submodule IN LISTS BOOST_SUBMODULES)
    include_directories(SYSTEM ${boost_SOURCE_DIR}/${submodule}/include)
endforeach()

FetchContent_Declare(
    json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         v3.10.4
    GIT_SHALLOW     TRUE
)
FetchContent_Populate(json)
include_directories(SYSTEM ${json_SOURCE_DIR}/single_include)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY  https://github.com/juce-framework/JUCE.git
    GIT_TAG         6.1.4
    GIT_SHALLOW     TRUE
)
FetchContent_Populate(JUCE)
add_subdirectory(${juce_SOURCE_DIR} ${juce_BINARY_DIR} EXCLUDE_FROM_ALL)

FetchContent_Declare(
    lager
    GIT_REPOSITORY  https://github.com/arximboldi/lager.git
    GIT_TAG         c5588f04405d4d0bf85bd587dd90516b24b1ec65
)
FetchContent_Populate(lager)
include_directories(SYSTEM ${lager_SOURCE_DIR})

FetchContent_Declare(
    websocketpp
    GIT_REPOSITORY  https://github.com/zaphoyd/websocketpp.git
    GIT_TAG         0.8.2
    GIT_SHALLOW     TRUE
)
FetchContent_Populate(websocketpp)
include_directories(SYSTEM ${websocketpp_SOURCE_DIR})

FetchContent_Declare(
    zug
    GIT_REPOSITORY  https://github.com/arximboldi/zug.git
    GIT_TAG         033dadbed463ff3430b7ebc36df8a46f4f0f5078
)
FetchContent_Populate(zug)
include_directories(SYSTEM ${zug_SOURCE_DIR})
