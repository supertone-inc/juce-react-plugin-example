import Knob from "./Knob";
import LevelView from "./LevelView";
import SpectrumView from "./SpectrumView";
import { StoreProvider, createWebSocketStore } from "./store";

const port = new URL(window.location.href).searchParams.get("port");

const store = createWebSocketStore(`ws://localhost:${port}`);

export default function App() {
  return (
    <StoreProvider store={store}>
      {({ state, dispatch }) => (
        <div
          style={{
            display: "flex",
            flexFlow: "row nowrap",
            height: "100vh",
          }}
        >
          <SpectrumView style={{ flex: 1 }} spectrum={state?.spectrum} />
          <LevelView style={{ width: 50 }} level={state?.level} />
          <Knob
            style={{
              position: "absolute",
              left: "50%",
              top: "50%",
              transform: "translate(-50%, -50%)",
            }}
            label="Gain"
            value={state?.parameters?.gain}
            onChange={(value) =>
              dispatch({ type: "UPDATE_PARAMETERS", payload: { gain: value } })
            }
          />

          <div
            style={{
              position: "fixed",
              left: 0,
              bottom: 0,
              margin: 10,
              fontSize: 10,
              WebkitUserSelect: "all",
              userSelect: "all",
            }}
          >
            {window.location.href}
          </div>
        </div>
      )}
    </StoreProvider>
  );
}
