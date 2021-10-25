start: build
    open "./build/AudioPluginHost/AudioPluginHost_artefacts/AudioPluginHost.app"

build:
    cmake -S . -B build
    cmake --build build
