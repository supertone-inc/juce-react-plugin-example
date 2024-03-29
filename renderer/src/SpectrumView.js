import React from "react";

export default function SpectrumView({ spectrum, style, ...props }) {
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

    const scopeSize = spectrum.length / 4;
    const barGap = 1;
    const barWidth = canvas.width / scopeSize - barGap;
    canvasCtx.fillStyle = "#f00";

    for (var i = 0, x = 0; i < scopeSize; i++, x += barWidth + barGap) {
      const skewedProportionX = 1 - Math.exp(Math.log(1 - i / scopeSize) * 0.2);
      const remappedIndex = Math.min(
        spectrum.length,
        (skewedProportionX * spectrum.length).toFixed()
      );

      const barHeight = canvas.height * spectrum[remappedIndex];

      canvasCtx.fillRect(x, canvas.height - barHeight, barWidth, barHeight);
    }
  }, [spectrum]);

  return <canvas ref={canvasRef} style={{ ...style }} {...props} />;
}
