"use strict";

var canvas;
var gl;

var theta = 0.0;
var thetaLoc;

let direction = 1;
let multiplier = 1;

var colors = [];
var cBuffer;
window.onload = function init() {
  canvas = document.getElementById("gl-canvas");

  gl = WebGLUtils.setupWebGL(canvas);
  if (!gl) {
    alert("WebGL isn't available");
  }

  //
  //  Configure WebGL
  //
  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(1.0, 1.0, 1.0, 1.0);

  //  Load shaders and initialize attribute buffers
  var program = initShaders(gl, "vertex-shader", "fragment-shader");
  gl.useProgram(program);

  var vertices = [
    vec2(-1.7320508 / 6, 0.5),
    vec2(1.7320508 / 3, 0),
    vec2(-1.7320508 / 6, -0.5),
  ];

  colors = [];
  colors.push(getRandomColor());
  colors.push(getRandomColor());
  colors.push(getRandomColor());

  // Load the data into the GPU
  var bufferId = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferId);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(vertices), gl.STATIC_DRAW);

  // Associate out shader variables with our data buffer
  var vPosition = gl.getAttribLocation(program, "vPosition");
  gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);

  cBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);

  var vColor = gl.getAttribLocation(program, "vColor");
  gl.vertexAttribPointer(vColor, 3, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vColor);

  thetaLoc = gl.getUniformLocation(program, "theta");

  render();
};

function toggleDirection() {
  direction = direction * -1;
}

function speedUp() {
  multiplier = multiplier * 2;
}

function slowDown() {
  multiplier = multiplier / 2;
}

function randomizeColors() {
  colors = [];
  colors.push(getRandomColor());
  colors.push(getRandomColor());
  colors.push(getRandomColor());
  gl.bufferData(gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);
}

function getRandomColor() {
  return vec3(Math.random(), Math.random(), Math.random());
}

function render() {
  gl.clear(gl.COLOR_BUFFER_BIT);

  theta += 0.04 * direction * multiplier;
  gl.uniform1f(thetaLoc, theta);

  gl.drawArrays(gl.LINE_LOOP, 0, 3);

  window.requestAnimFrame(render);
}
