include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

set(BOOST_TAG boost-1.78.0)
set(BOOST_LIBS
    asio
    assert
    bind
    config
    core
    date_time
    hana
    intrusive
    move
    mpl
    numeric_conversion
    predef
    preprocessor
    regex
    smart_ptr
    static_assert
    system
    throw_exception
    utility
    type_traits
)
foreach(lib IN LISTS BOOST_LIBS)
    FetchContent_Declare(
        boost_${lib}
        GIT_REPOSITORY  https://github.com/boostorg/${lib}.git
        GIT_TAG         ${BOOST_TAG}
        GIT_SHALLOW     TRUE
    )
    FetchContent_Populate(boost_${lib})
    include_directories(SYSTEM ${boost_${lib}_SOURCE_DIR}/include)
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
    GIT_TAG         6.1.2
    GIT_SHALLOW     TRUE
)
FetchContent_Populate(JUCE)
add_subdirectory(${juce_SOURCE_DIR} ${juce_BINARY_DIR} EXCLUDE_FROM_ALL)

FetchContent_Declare(
    lager
    GIT_REPOSITORY  https://github.com/arximboldi/lager.git
    GIT_TAG         c5588f04405d4d0bf85bd587dd90516b24b1ec65
    GIT_SHALLOW     TRUE
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
    GIT_SHALLOW     TRUE
)
FetchContent_Populate(zug)
include_directories(SYSTEM ${zug_SOURCE_DIR})
