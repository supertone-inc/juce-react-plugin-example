RUN_STANDALONE_PLUGIN := if os() == "windows" {
    "./build/AudioPluginExample_artefacts/Debug/Standalone/Audio Plugin Example.exe"
} else {
    "./build/AudioPluginExample_artefacts/Debug/Standalone/Audio Plugin Example.app/Contents/MacOS/Audio Plugin Example"
}

RENDERER_BUILD_PATH := if os() == "windows" {
    "../build/AudioPluginExample_artefacts/Release/Standalone/renderer"
} else {
    "../build/AudioPluginExample_artefacts/Release/Standalone/Audio Plugin Example.app/Contents/MacOS/renderer"
}

start:
    just start-main & just start-renderer

start-renderer:
    BROWSER=none yarn --cwd renderer start

start-main:
    cmake -S main -B build -D CMAKE_BUILD_TYPE=Debug
    cmake --build build --config Debug
    "{{RUN_STANDALONE_PLUGIN}}"

build: build-renderer build-main

build-renderer:
    BUILD_PATH="{{RENDERER_BUILD_PATH}}" yarn --cwd renderer build

build-main:
    cmake -S main -B build -D CMAKE_BUILD_TYPE=Release
    cmake --build build --config Release
