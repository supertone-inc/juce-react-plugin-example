import React from "react";

import StoreContext from "./StoreContext";

export default function StoreProvider({ store, children, ...props }) {
  const [state, setState] = React.useState(null);

  React.useEffect(() => store.subscribe(setState), [store]);

  return (
    <StoreContext.Provider
      value={{
        state,
        dispatch: store.dispatch,
      }}
      {...props}
    >
      {children instanceof Function ? (
        <StoreContext.Consumer>{children}</StoreContext.Consumer>
      ) : (
        children
      )}
    </StoreContext.Provider>
  );
}
