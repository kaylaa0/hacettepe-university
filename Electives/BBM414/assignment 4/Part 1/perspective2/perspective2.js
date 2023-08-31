/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

var canvas;
var gl;

var NumVertices = 36;

var pointsArray = [];
var colorsArray = [];

var vertices = [
  vec4(-0.5, -0.5, 1.5, 1.0),
  vec4(-0.5, 0.5, 1.5, 1.0),
  vec4(0.5, 0.5, 1.5, 1.0),
  vec4(0.5, -0.5, 1.5, 1.0),
  vec4(-0.5, -0.5, 0.5, 1.0),
  vec4(-0.5, 0.5, 0.5, 1.0),
  vec4(0.5, 0.5, 0.5, 1.0),
  vec4(0.5, -0.5, 0.5, 1.0),
];

var vertexColors = [
  vec4(0.0, 0.0, 0.0, 1.0), // black
  vec4(1.0, 0.0, 0.0, 1.0), // red
  vec4(1.0, 1.0, 0.0, 1.0), // yellow
  vec4(0.0, 1.0, 0.0, 1.0), // green
  vec4(0.0, 0.0, 1.0, 1.0), // blue
  vec4(1.0, 0.0, 1.0, 1.0), // magenta
  vec4(0.0, 1.0, 1.0, 1.0), // cyan
  vec4(1.0, 1.0, 1.0, 1.0), // white
];

var near = 0.3;
var far = 10;
var radius = 5;
var theta = 0.0;
var phi = 0.0;
var dr = (5.0 * Math.PI) / 180.0;

var fovy = 60; // Field-of-view in Y direction angle (in degrees)
var aspect = 1.0; // Viewport aspect ratio

var modelViewMatrix, projectionMatrix;
var modelViewMatrixLoc, projectionMatrixLoc;
var eye;
const at = vec3(0.0, 0.0, 0.0);
const up = vec3(0.0, 1.0, 0.0);

function quad(a, b, c, d) {
  pointsArray.push(vertices[a]);
  colorsArray.push(vertexColors[a]);
  pointsArray.push(vertices[b]);
  colorsArray.push(vertexColors[a]);
  pointsArray.push(vertices[c]);
  colorsArray.push(vertexColors[a]);
  pointsArray.push(vertices[a]);
  colorsArray.push(vertexColors[a]);
  pointsArray.push(vertices[c]);
  colorsArray.push(vertexColors[a]);
  pointsArray.push(vertices[d]);
  colorsArray.push(vertexColors[a]);
}

function colorCube() {
  quad(1, 0, 3, 2);
  quad(2, 3, 7, 6);
  quad(3, 0, 4, 7);
  quad(6, 5, 1, 2);
  quad(4, 5, 6, 7);
  quad(5, 4, 0, 1);
}

let entrycoordinates = { x: -1, y: -1 };

function getPosition(canvas, event) {
  var x;
  var y;

  if (event.x != undefined && event.y != undefined) {
    x = event.x;
    y = event.y;
  } // firefox method to get the position
  else {
    x =
      event.clientX +
      document.body.scrollLeft +
      document.documentElement.scrollLeft;
    y =
      event.clientY +
      document.body.scrollTop +
      document.documentElement.scrollTop;
  }

  x -= canvas.offsetLeft;
  y -= canvas.offsetTop;

  return { x: x, y: y };
}

// My variables are here:
var isLocked = false; // Status of pointer lock

function movementHandler(e) {
  var movementX = e.movementX || e.mozmovementX || e.webkitmovementX || 0;

  var movementY = e.movementY || e.mozmovementY || e.webkitmovementY || 0;

  theta += -movementX / 100;
  phi += -movementY / 100;
}

function changeHandler(e) {}

var render = function () {
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  eye = vec3(
    radius * Math.sin(theta) * Math.cos(phi),
    radius * Math.sin(theta) * Math.sin(phi),
    radius * Math.cos(theta)
  );
  modelViewMatrix = lookAt(eye, at, up);
  projectionMatrix = perspective(fovy, aspect, near, far);

  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));

  gl.drawArrays(gl.TRIANGLES, 0, NumVertices);
  requestAnimFrame(render);
};

window.onload = function init() {
  canvas = document.getElementById("gl-canvas");

  gl = WebGLUtils.setupWebGL(canvas);
  if (!gl) {
    alert("WebGL2 isn't available");
  }

  gl.viewport(0, 0, canvas.width, canvas.height);

  aspect = canvas.width / canvas.height;

  // Here we hook the pointer lock
  let checkPointerLock =
    "pointerLockElement" in document ||
    "mozPointerLockElement" in document ||
    "webkitPointerLockElement" in document;

  // We always check triple for every kind of browser.
  document.addEventListener("pointerLockChange", changeHandler);
  document.addEventListener("mozPointerLockChange", changeHandler);
  document.addEventListener("webkitPointerLockChange", changeHandler);

  document.addEventListener("keydown", function (event) {
    // Hooking keyboard inputs to change state pointer lock.
    if (event.code === "KeyP") {
      if (!isLocked) {
        if (checkPointerLock) {
          // If pointer lock exist we lock it.
          canvas.requestPointerLock =
            canvas.requestPointerLock ||
            canvas.mozRequestPointerLock ||
            canvas.webkitRequestPointerLock;
          canvas.requestPointerLock();

          document.addEventListener("mousemove", movementHandler, false); // Hooking mouse move

          isLocked = true;
        }
      } else {
        canvas.exitPointerLock =
          canvas.exitPointerLock ||
          canvas.mozExitPointerLock ||
          canvas.webkitExitPointerLock;
        document.exitPointerLock(); // Exiting pointer lock
        document.removeEventListener("mousemove", movementHandler, false); // Removing hook on mouse movement
        isLocked = false;
      }
    }
  });

  gl.clearColor(1.0, 1.0, 1.0, 1.0);

  gl.enable(gl.DEPTH_TEST);

  //
  //  Load shaders and initialize attribute buffers
  //
  var program = initShaders(gl, "vertex-shader", "fragment-shader");
  gl.useProgram(program);

  colorCube();

  var cBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(colorsArray), gl.STATIC_DRAW);

  var vColor = gl.getAttribLocation(program, "vColor");
  gl.vertexAttribPointer(vColor, 4, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vColor);

  var vBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(pointsArray), gl.STATIC_DRAW);

  var vPosition = gl.getAttribLocation(program, "vPosition");
  gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);

  modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");
  projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");

  // sliders for viewing parameters

  document.getElementById("zFarSlider").onchange = function () {
    far = document.getElementById("zFarSlider").value;
  };
  document.getElementById("zNearSlider").onchange = function () {
    near = event.srcElement.value;
  };
  document.getElementById("radiusSlider").onchange = function () {
    radius = event.srcElement.value;
  };
  document.getElementById("aspectSlider").onchange = function () {
    aspect = event.srcElement.value;
  };
  document.getElementById("fovSlider").onchange = function () {
    fovy = event.srcElement.value;
  };

  render();
};
