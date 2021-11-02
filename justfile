RUN_STANDALONE_PLUGIN := if os() == "windows" {
    "./build/AudioPluginExample_artefacts/Debug/Standalone/Audio Plugin Example.exe"
} else {
    "./build/AudioPluginExample_artefacts/Standalone/Audio Plugin Example.app/Contents/MacOS/Audio Plugin Example"
}

start:
    just start-renderer & just start-plugin

start-renderer:
    yarn --cwd renderer start

start-plugin: build
    "{{RUN_STANDALONE_PLUGIN}}"

build:
    cmake -S plugin -B build
    cmake --build build
