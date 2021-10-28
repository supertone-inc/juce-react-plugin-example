RUN_STANDALONE_PLUGIN := if os() == "windows" {
    "./build/plugin/AudioPluginExample_artefacts/Debug/Standalone/Audio Plugin Example.exe"
} else {
    "./build/plugin/AudioPluginExample_artefacts/Standalone/Audio Plugin Example.app/Contents/MacOS/Audio Plugin Example"
}

start: build
    "{{RUN_STANDALONE_PLUGIN}}"

build:
    cmake -S . -B build
    cmake --build build
