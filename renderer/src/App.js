import Level from "./Level";
import Spectrum from "./Spectrum";
import { createWebSocketStore, StoreProvider } from "./store";

const websocketPort = new URL(window.location.href).searchParams.get("port");

const store = createWebSocketStore(`ws://localhost:${websocketPort}`);

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
          <Spectrum style={{ flex: 1 }} spectrum={state?.spectrum} />
          <Level style={{ width: 20 }} level={state?.level} />

          <div style={{ position: "fixed", left: 0, bottom: 0, margin: 10 }}>
            WebSocket Port: {websocketPort}
          </div>
        </div>
      )}
    </StoreProvider>
  );
}
