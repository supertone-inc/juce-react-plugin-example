start: build
    open "./build/plugin-host/AudioPluginHost_artefacts/AudioPluginHost.app" --args "$(pwd)/example.filtergraph"

build:
    cmake -S . -B build
    cmake --build build
