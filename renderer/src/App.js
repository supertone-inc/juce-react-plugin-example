import LevelView from "./LevelView";
import SpectrumView from "./SpectrumView";
import { StoreProvider, createWebSocketStore } from "./store";

const port = new URL(window.location.href).searchParams.get("port");

const store = createWebSocketStore(`ws://localhost:${port}`);

export default function App() {
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
          <LevelView style={{ width: 20 }} level={state?.level} />

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
