const child_process = require("child_process");

function execSync(command, options) {
  return child_process.execSync(command, { stdio: "inherit", ...options });
}

const script = process.argv[2];

switch (script) {
  case "build-main":
    execSync("cmake -S main -B build -D CMAKE_BUILD_TYPE=Release");
    execSync("cmake --build build --config Release");
    break;

  case "build-renderer":
    const build_path =
      process.platform == "win32"
        ? "../build/AudioPluginExample_artefacts/Release/Standalone/renderer"
        : "../build/AudioPluginExample_artefacts/Release/Standalone/Audio Plugin Example.app/Contents/MacOS/renderer";

    execSync("yarn --cwd renderer build", {
      env: { ...process.env, BUILD_PATH: build_path },
    });
    break;

  case "start-main":
    execSync("cmake -S main -B build -D CMAKE_BUILD_TYPE=Debug");
    execSync("cmake --build build --config Debug");

    const executable_path =
      process.platform == "win32"
        ? "./build/AudioPluginExample_artefacts/Debug/Standalone/Audio Plugin Example.exe"
        : "./build/AudioPluginExample_artefacts/Debug/Standalone/Audio Plugin Example.app/Contents/MacOS/Audio Plugin Example";

    execSync(`"${executable_path}"`);
    break;

  case "start-renderer":
    execSync("yarn --cwd renderer start", {
      env: { ...process.env, BROWSER: "none" },
    });
    break;
}
