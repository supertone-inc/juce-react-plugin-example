# juce-react-plugin-example

Developing a JUCE plugin with React.js.

## Requirements

- [CMake](https://cmake.org/download) (See [this](https://cgold.readthedocs.io/en/latest/first-step/installation.html) for installation.)
- [Node.js](https://nodejs.org) (Using [nvm](https://github.com/nvm-sh/nvm) is recommended.)
- [Yarn](https://yarnpkg.com/getting-started/install)

## Getting Started

### Installing Dependencies

```shell
yarn install
```

### Developing the Plugins

```shell
yarn start
```

This will run the React app in the development mode and run the standalone plugin in debug mode:

### Building the Plugins

```shell
yarn build
```

This will build the React app and the plugin for release. The resulting static web files of the React app will be copied into the plugin's internal directory.

## Audio Routing

To send audio signal to the standalone plugin from other than microphone (like audio files or etc.), consider using the virtual audio devices like:

- [BlackHole](https://github.com/ExistentialAudio/BlackHole) (for macOS)
- [VB-CABLE](https://vb-audio.com/Cable) (for macOS and Windows)
