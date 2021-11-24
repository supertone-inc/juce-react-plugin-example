export default function createWebSocketStore(url, protocols) {
  const socket = new WebSocket(url, protocols);

  const callbacks = new Set();

  let lastState = null;

  socket.onmessage = (event) => {
    lastState = JSON.parse(event.data);

    for (const callback of callbacks) {
      callback(lastState);
    }
  };

  return {
    subscribe(callback) {
      callbacks.add(callback);
      callback(lastState);

      return () => {
        callbacks.delete(callback);
      };
    },
    dispatch(action) {
      socket.send(action);
    },
  };
}
