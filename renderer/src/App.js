import LevelMeter from "./LevelMeter";
import { createWebSocketStore, StoreProvider } from "./store";

const websocketPort = new URL(window.location.href).searchParams.get("port");

const store = createWebSocketStore(`ws://localhost:${websocketPort}`);

export default function App() {
  return (
    <StoreProvider store={store}>
      {({ state }) => (
        <div style={{ height: "100vh" }}>
          <LevelMeter style={{ height: "100%" }} level={state?.level} />

          <div style={{ position: "fixed", left: 0, bottom: 0, margin: 10 }}>
            WebSocket Port: {websocketPort}
          </div>
        </div>
      )}
    </StoreProvider>
  );
}
