start: build
    open "./build/AudioPluginHost/AudioPluginHost_artefacts/AudioPluginHost.app" --args "$(pwd)/example.filtergraph"

build:
    cmake -S . -B build
    cmake --build build
