export default function createWebSocketStore(url, protocols) {
  const socket = new WebSocket(url, protocols);

  const callbacks = new Set();

  let lastMessage = null;

  socket.onmessage = (event) => {
    lastMessage = JSON.parse(event.data);

    for (const callback of callbacks) {
      callback(lastMessage);
    }
  };

  return {
    subscribe(callback) {
      callbacks.add(callback);
      callback(lastMessage);

      return () => {
        callbacks.delete(callback);
      };
    },
    dispatch(action) {
      socket.send(action);
    },
  };
}
