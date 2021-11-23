import React from "react";

import StoreContext from "./StoreContext";

export default function useStore() {
  return React.useContext(StoreContext);
}
