# juce-react-plugin-example

Test and demonstrate how to develop a JUCE plugin with React.js.

## Prerequisites

- [CMake](https://cmake.org/download) (See [this](https://cgold.readthedocs.io/en/latest/first-step/installation.html) for installation.)
- [Node.js](https://nodejs.org) (Using [nvm](https://github.com/nvm-sh/nvm) is recommended.)
- [Yarn](https://yarnpkg.com/getting-started/install)

## Getting Started

Install dependencies:

```shell
yarn install
```

To run the standalone plugin in debug mode:

```shell
yarn start
```

To build the standalone plugin for release:

```shell
yarn build
```

## Audio Routing

To send audio signal to the standalone plugin from other than microphone (like audio files or etc.), consider using the virtual audio devices like:

- [BlackHole](https://github.com/ExistentialAudio/BlackHole) (for macOS)
- [VB-CABLE](https://vb-audio.com/Cable) (for macOS and Windows)
