OPEN_PLUGIN_HOST := if os() == "windows" {
    "start ./build/plugin-host/AudioPluginHost_artefacts/Debug/AudioPluginHost.exe"
} else {
    "open ./build/plugin-host/AudioPluginHost_artefacts/AudioPluginHost.app --args"
}

start: build
    {{OPEN_PLUGIN_HOST}} "{{invocation_directory()}}/example.filtergraph"

build:
    cmake -S . -B build
    cmake --build build
