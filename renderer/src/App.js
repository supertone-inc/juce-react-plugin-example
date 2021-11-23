const websocketPort = new URL(window.location.href).searchParams.get("port");

export default function App() {
  return (
    <div
      style={{
        display: "flex",
        justifyContent: "center",
        alignItems: "center",
        height: "100vh",
      }}
    >
      WebSocket Port: {websocketPort}
    </div>
  );
}
