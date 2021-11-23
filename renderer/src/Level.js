export default function Level({ level, style, ...props }) {
  return (
    <div style={{ ...style }} {...props}>
      <div
        style={{
          height: "100%",
          backgroundColor: "#f00",
          transform: `scaleY(${level || 0})`,
          transformOrigin: "bottom",
        }}
      />
    </div>
  );
}
