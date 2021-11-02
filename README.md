# juce-rust-react-plugin-example

Test and demonstrate how to develop a plugin with **React.js** for GUI and **Rust** for core logic.

## Prerequisites

- [Rust](https://www.rust-lang.org/tools/install)
- [just](https://github.com/casey/just#installation)
- [CMake](https://cmake.org/download) (See [this](https://cgold.readthedocs.io/en/latest/first-step/installation.html) for installation.)

## Getting Started

To build and run a standalone plugin:

```shell
just start
```

### Audio Routing

To send an audio signal from other than microphone (like audio files or etc.) to the plugin, consider using the virtual audio devices like:

- [BlackHole](https://github.com/ExistentialAudio/BlackHole) (for macOS)
- [VB-CABLE](https://vb-audio.com/Cable) (for macOS and Windows)
