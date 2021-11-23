import LevelMeter from "./LevelMeter";

const websocketPort = new URL(window.location.href).searchParams.get("port");

export default function App() {
  return (
    <div style={{ height: "100vh" }}>
      <LevelMeter style={{ height: "100%" }} level={0.5} />

      <div style={{ position: "fixed", left: 0, bottom: 0, margin: 10 }}>
        WebSocket Port: {websocketPort}
      </div>
    </div>
  );
}
