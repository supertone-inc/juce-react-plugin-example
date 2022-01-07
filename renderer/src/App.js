import React from "react";

import GainKnob from "./GainKnob";
import LevelView from "./LevelView";
import SpectrumView from "./SpectrumView";
import { StoreProvider, createWebSocketStore } from "./store";

const port = new URL(window.location.href).searchParams.get("port");

const store = createWebSocketStore(`ws://localhost:${port}`);

export default function App() {
  const [gain, setGain] = React.useState(0.5);

  return (
    <StoreProvider store={store}>
      {({ state }) => (
        <div
          style={{
            display: "flex",
            flexFlow: "row nowrap",
            height: "100vh",
          }}
        >
          <SpectrumView style={{ flex: 1 }} spectrum={state?.spectrum} />
          <LevelView style={{ width: 50 }} level={state?.level} />
          <GainKnob
            style={{
              position: "absolute",
              left: "50%",
              top: "50%",
              transform: "translate(-50%, -50%)",
            }}
            gain={gain}
            onChange={setGain}
          />

          <div
            style={{
              position: "fixed",
              left: 0,
              bottom: 0,
              margin: 10,
              fontSize: 10,
            }}
          >
            {window.location.href}
          </div>
        </div>
      )}
    </StoreProvider>
  );
}
