const { execSync } = require("child_process");

const BUILD_PATH =
  process.platform == "win32"
    ? "../build/AudioPluginExample_artefacts/Release/Standalone/renderer"
    : "../build/AudioPluginExample_artefacts/Release/Standalone/Audio Plugin Example.app/Contents/MacOS/renderer";

execSync("yarn react-scripts build", {
  env: { ...process.env, BUILD_PATH },
  stdio: "inherit",
});
