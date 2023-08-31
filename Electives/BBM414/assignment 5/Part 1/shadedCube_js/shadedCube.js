/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

/*
 I started with modifying the toggle button and adding other buttons. There was already an array which holds the light position.
 Connected new buttons to update the array's element and sending data to shader.
 Implemented the texture projecting, added elements to shader and implemented a binder function. Configured quad function to add texture positions and it was done.
*/
var canvas;
var gl;

var numVertices = 36;

var pointsArray = [];
var normalsArray = [];
var texCoordsArray = [];

var texture;

var vertices = [
  vec4(-0.5, -0.5, 0.5, 1.0),
  vec4(-0.5, 0.5, 0.5, 1.0),
  vec4(0.5, 0.5, 0.5, 1.0),
  vec4(0.5, -0.5, 0.5, 1.0),
  vec4(-0.5, -0.5, -0.5, 1.0),
  vec4(-0.5, 0.5, -0.5, 1.0),
  vec4(0.5, 0.5, -0.5, 1.0),
  vec4(0.5, -0.5, -0.5, 1.0),
];

var lightPosition = vec4(1.0, 1.0, 1.0, 0.0);
var lightAmbient = vec4(0.2, 0.2, 0.2, 1.0);
var lightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
var lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);

var materialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
var materialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
var materialSpecular = vec4(1.0, 1.0, 1.0, 1.0);
var materialShininess = 100.0;

var ctm;
var ambientColor, diffuseColor, specularColor;
var modelView, projection;
var viewerPos;
var program;

var xAxis = 0;
var yAxis = 1;
var zAxis = 2;
var axis = 0;
var theta = [0, 0, 0];

var thetaLoc;

var flag = true;

function bindTexture(texImage, position) {
  texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);
  gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, texImage);
  gl.generateMipmap(gl.TEXTURE_2D);
  gl.texParameteri(
    gl.TEXTURE_2D,
    gl.TEXTURE_MIN_FILTER,
    gl.NEAREST_MIPMAP_LINEAR
  );
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

  gl.uniform1i(gl.getUniformLocation(program, position), 0);
}

function quad(a, b, c, d) {
  var t1 = subtract(vertices[b], vertices[a]);
  var t2 = subtract(vertices[c], vertices[b]);
  var normal = cross(t1, t2);
  var normal = vec3(normal);

  pointsArray.push(vertices[a]);
  normalsArray.push(normal);
  texCoordsArray.push(vec2(0, 0));
  pointsArray.push(vertices[b]);
  normalsArray.push(normal);
  texCoordsArray.push(vec2(0, 1));
  pointsArray.push(vertices[c]);
  normalsArray.push(normal);
  texCoordsArray.push(vec2(1, 1));
  pointsArray.push(vertices[a]);
  normalsArray.push(normal);
  texCoordsArray.push(vec2(0, 0));
  pointsArray.push(vertices[c]);
  normalsArray.push(normal);
  texCoordsArray.push(vec2(1, 1));
  pointsArray.push(vertices[d]);
  normalsArray.push(normal);
  texCoordsArray.push(vec2(1, 0));
}

function colorCube() {
  quad(1, 0, 3, 2);
  quad(2, 3, 7, 6);
  quad(3, 0, 4, 7);
  quad(6, 5, 1, 2);
  quad(4, 5, 6, 7);
  quad(5, 4, 0, 1);
}

window.onload = function init() {
  canvas = document.getElementById("gl-canvas");

  //gl = WebGLUtils.setupWebGL( canvas );
  gl = canvas.getContext("webgl2");
  if (!gl) {
    alert("WebGL isn't available");
  }

  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(1.0, 1.0, 1.0, 1.0);

  gl.enable(gl.DEPTH_TEST);

  //
  //  Load shaders and initialize attribute buffers
  //
  program = initShaders(gl, "vertex-shader", "fragment-shader");
  gl.useProgram(program);

  colorCube();

  var nBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, nBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(normalsArray), gl.STATIC_DRAW);

  var vNormal = gl.getAttribLocation(program, "vNormal");
  gl.vertexAttribPointer(vNormal, 3, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vNormal);

  var vBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(pointsArray), gl.STATIC_DRAW);

  var vPosition = gl.getAttribLocation(program, "vPosition");
  gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);

  var tBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, tBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(texCoordsArray), gl.STATIC_DRAW);

  var vTexCoord = gl.getAttribLocation(program, "vTexCoord");
  gl.vertexAttribPointer(vTexCoord, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vTexCoord);

  var texImage = document.getElementById("Texture");

  bindTexture(texImage, "Texture");

  thetaLoc = gl.getUniformLocation(program, "theta");

  viewerPos = vec3(0.0, 0.0, -20.0);

  projection = ortho(-1, 1, -1, 1, -100, 100);

  ambientProduct = mult(lightAmbient, materialAmbient);
  diffuseProduct = mult(lightDiffuse, materialDiffuse);
  specularProduct = mult(lightSpecular, materialSpecular);

  document.getElementById("ButtonX").onclick = function () {
    axis = xAxis;
  };
  document.getElementById("ButtonY").onclick = function () {
    axis = yAxis;
  };
  document.getElementById("ButtonZ").onclick = function () {
    axis = zAxis;
  };
  document.getElementById("ButtonT").onclick = function () {
    flag = !flag;
  };
  document.getElementById("ButtonLXPlus").onclick = function () {
    lightPosition[0] += 0.1;
    gl.uniform4fv(
      gl.getUniformLocation(program, "lightPosition"),
      flatten(lightPosition)
    );
  };
  document.getElementById("ButtonLXMinus").onclick = function () {
    lightPosition[0] -= 0.1;
    gl.uniform4fv(
      gl.getUniformLocation(program, "lightPosition"),
      flatten(lightPosition)
    );
  };
  document.getElementById("ButtonLYPlus").onclick = function () {
    lightPosition[1] += 0.1;
    gl.uniform4fv(
      gl.getUniformLocation(program, "lightPosition"),
      flatten(lightPosition)
    );
  };
  document.getElementById("ButtonLYMinus").onclick = function () {
    lightPosition[1] -= 0.1;
    gl.uniform4fv(
      gl.getUniformLocation(program, "lightPosition"),
      flatten(lightPosition)
    );
  };
  document.getElementById("ButtonLZPlus").onclick = function () {
    lightPosition[2] += 0.1;
    gl.uniform4fv(
      gl.getUniformLocation(program, "lightPosition"),
      flatten(lightPosition)
    );
  };
  document.getElementById("ButtonLZMinus").onclick = function () {
    lightPosition[2] -= 0.1;
    gl.uniform4fv(
      gl.getUniformLocation(program, "lightPosition"),
      flatten(lightPosition)
    );
  };

  gl.uniform4fv(
    gl.getUniformLocation(program, "ambientProduct"),
    flatten(ambientProduct)
  );
  gl.uniform4fv(
    gl.getUniformLocation(program, "diffuseProduct"),
    flatten(diffuseProduct)
  );
  gl.uniform4fv(
    gl.getUniformLocation(program, "specularProduct"),
    flatten(specularProduct)
  );
  gl.uniform4fv(
    gl.getUniformLocation(program, "lightPosition"),
    flatten(lightPosition)
  );

  gl.uniform1f(gl.getUniformLocation(program, "shininess"), materialShininess);

  gl.uniformMatrix4fv(
    gl.getUniformLocation(program, "projectionMatrix"),
    false,
    flatten(projection)
  );

  render();
};

var render = function () {
  gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

  if (flag) theta[axis] += 2.0;

  modelView = mat4();
  modelView = mult(modelView, rotate(theta[xAxis], [1, 0, 0]));
  modelView = mult(modelView, rotate(theta[yAxis], [0, 1, 0]));
  modelView = mult(modelView, rotate(theta[zAxis], [0, 0, 1]));

  gl.uniformMatrix4fv(
    gl.getUniformLocation(program, "modelViewMatrix"),
    false,
    flatten(modelView)
  );

  gl.drawArrays(gl.TRIANGLES, 0, numVertices);

  requestAnimFrame(render);
};