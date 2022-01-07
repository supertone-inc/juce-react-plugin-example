import React from "react";

const pointOnCircle = (center, radius, angle) => ({
  x: center + radius * Math.cos(angle),
  y: center + radius * Math.sin(angle),
});
const degTorad = (deg) => (Math.PI * deg) / 180;

const calcPath = ({
  percentage,
  angleOffset,
  angleRange,
  thickness: arcWidth,
  radius: outerRadius,
  center,
}) => {
  const angle = angleRange * percentage;
  const startAngle = angleOffset - 90;
  const innerRadius = outerRadius - arcWidth;
  const startAngleDegree = degTorad(startAngle);
  const endAngleDegree = degTorad(startAngle + angle);
  const largeArcFlag = angle < 180 ? 0 : 1;

  const p1 = pointOnCircle(center, outerRadius, endAngleDegree);
  const p2 = pointOnCircle(center, outerRadius, startAngleDegree);
  const p3 = pointOnCircle(center, innerRadius, startAngleDegree);
  const p4 = pointOnCircle(center, innerRadius, endAngleDegree);

  return `M${p1.x},${p1.y} A${outerRadius},${outerRadius} 0 ${largeArcFlag} 0 ${p2.x},${p2.y}L${p3.x},${p3.y} A${innerRadius},${innerRadius} 0 ${largeArcFlag} 1 ${p4.x},${p4.y} L${p1.x},${p1.y}`;
};

function Arc({ color, background, ...props }) {
  return (
    <g>
      {background && (
        <path
          d={calcPath({ ...props, percentage: 1 })}
          style={{ fill: background }}
        />
      )}
      <path d={calcPath(props)} style={{ fill: color }} />
    </g>
  );
}

function Knob({
  value,
  min = 0,
  max = 1,
  valuePerPixel = 0.005,
  size = 100,
  color = "#f00",
  background = "rgba(255, 0, 0, 0.25)",
  angleOffset = 220,
  angleRange = 280,
  thickness = 10,
  style,
  onChange,
  ...props
}) {
  function handleMouseDown(event) {
    const initialY = event.pageY;

    function handleMouseMove(event) {
      const y = event.pageY;
      const dValue = (initialY - y) * valuePerPixel;
      const newValue = Math.min(max, Math.max(min, value + dValue));

      onChange?.(newValue);
    }

    function handleMouseUp() {
      window.removeEventListener("mousemove", handleMouseMove);
    }

    window.addEventListener("mousemove", handleMouseMove);
    window.addEventListener("mouseup", handleMouseUp);
  }

  return (
    <svg
      style={{ cursor: "ns-resize", ...style }}
      width={size}
      height={size}
      onMouseDown={handleMouseDown}
      {...props}
    >
      <Arc
        color={color}
        background={background}
        angleOffset={angleOffset}
        angleRange={angleRange}
        thickness={thickness}
        radius={size / 2}
        center={size / 2}
        percentage={(value - min) / (max - min)}
      />
    </svg>
  );
}

export default function GainKnob({ gain, ...props }) {
  return <Knob value={gain} {...props} />;
}
