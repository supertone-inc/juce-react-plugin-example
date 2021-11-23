import React from "react";

export default function Spectrum({ spectrum, style, ...props }) {
  const canvasRef = React.useRef();

  React.useEffect(() => {
    if (!spectrum) {
      return;
    }

    const canvas = canvasRef.current;

    canvas.width = canvas.offsetWidth;
    canvas.height = canvas.offsetHeight;

    const canvasCtx = canvas.getContext("2d");

    canvasCtx.clearRect(0, 0, canvas.width, canvas.height);

    const barWidth = (2.5 * canvas.width) / spectrum.length;
    canvasCtx.fillStyle = "#f00";
    for (var i = 0, x = 0; i < spectrum.length; i++, x += barWidth + 1) {
      const barHeight = canvas.height * spectrum[i];

      canvasCtx.fillRect(x, canvas.height - barHeight, barWidth, barHeight);
    }
  }, [spectrum]);

  return <canvas ref={canvasRef} style={{ ...style }} {...props} />;
}
