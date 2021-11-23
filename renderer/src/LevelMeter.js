export default function LevelMeter({ level, style, ...props }) {
  return (
    <div style={{ ...style }} {...props}>
      <div
        style={{
          height: "100%",
          backgroundColor: "#f00",
          transform: `scaleY(${level})`,
          transformOrigin: "bottom",
        }}
      />
    </div>
  );
}
