"use strict";
/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Renderer {
  // These are used in renderer
  static GL = null;
  static Program = null;
  static Buffer = null;

  static spinState = 0; // Spin state of program
  static spinValue = document.getElementById("spinInput").value; // We read directly from html

  static scaleState = 0; // Scale state of program
  static currentScale = [0.3, 0.3]; // You can change this manually to scale.
  static scaleDirection = 1; // When reach max state we shrink

  static spiralState = 0; // Spiral state of program
  static spiralValue = document.getElementById("spiralInput").value; // We read value directly from html
  static ghostSpiralValue = 0; // This is used to determine sign flip of input value
  static transformAngle = 0; // Angle of transform
  static currentDirectionGrowth = 1; // Whether we are growing our distance from origin or shrinking
  static transformLength = [0, 0]; // Our current distance from origin, vector
  static transformRotation = [0, 1]; // Our current rotation vector
  static transformPosition = [0, 0]; // Our transform position
  static transformCurrentAngle = 0; // Current angle of transform

  static transformScale = [0.1, 0.1]; // We can scale down or up the transformation to fit the canvas. :)

  static currentAngle = 0.0; // Rotation angle of the emoji
  static currentRotation = [0, 1]; // Rotation vector of the emoji

  static previousTime = 0.0; // Previous time which is used to calculate animation time
  static degreesPerSecond = 45.0; // Rotation speed

  static rotationDirection = -1; // -1 is counter clock wise, 1 is clock wise. Multiplied with delta angle.

  static _Init() {
    // Initializer for Renderer
    const canvas = document.querySelector("#glCanvas");
    Renderer.GL = canvas.getContext("webgl2");
    const gl = Renderer.GL;

    if (!gl) {
      alert(
        "Unable to initialize WebGL2.0. Your browser or machine may not support it."
      );
      return;
    }

    gl.clearColor(1, 1, 1, 1.0); // White background for the canvas.
    gl.clearDepth(1.0); // Clearing the canvas.
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    Renderer.Program = initShaderProgram(gl, vShader, fShader); // Initializing the shader program.
    Renderer.Buffer = gl.createBuffer(); // Creating buffer.
    const buffer = Renderer.Buffer;
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer); // Binding buffer to an ARRAY_BUFFER.

    Renderer._AnimationLoop();
  }

  static render(gl, program, mode, NumVertices, vPositions, vColors, colorTo) {
    const numOfComponents = 4;
    const type = gl.FLOAT;
    const normalize = false;
    const stride = 0;
    var offset = 0;

    // Getting elements of vertex and color data and putting them in an array. Before this they were arrays of points/arrays now they are linear.
    var positionArray = [];
    for (var i = 0; i < NumVertices; i++) {
      positionArray = positionArray.concat(vPositions[i].elements());
    }
    var colorArray = [];
    for (var i = 0; i < NumVertices; i++) {
      colorArray = colorArray.concat(vColors[i].elements());
    }

    var data = positionArray.concat(colorArray); // Data is basically the vertex array and color array concatenated.

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(data), gl.STATIC_DRAW); // Binding current data to our buffer.
    gl.bufferSubData(gl.ARRAY_BUFFER, 0, new Float32Array(positionArray)); // Dividing buffer to positions.
    gl.bufferSubData(
      gl.ARRAY_BUFFER,
      positionArray.length * 4,
      new Float32Array(colorArray)
    );

    gl.useProgram(program);

    let radians = (Renderer.currentAngle * Math.PI) / 180.0; // Converting angle to radians.
    Renderer.currentRotation[0] = Math.sin(radians); // Calculating rotation vector.
    Renderer.currentRotation[1] = Math.cos(radians);

    // Passing variables to shaders.
    gl.uniform2fv(
      gl.getUniformLocation(program, "uScalingFactor"),
      Renderer.currentScale
    );
    gl.uniform2fv(
      gl.getUniformLocation(program, "uRotationVector"),
      Renderer.currentRotation
    );

    gl.uniform2fv(
      gl.getUniformLocation(program, "uTransformPosition"),
      Renderer.transformPosition
    );
    gl.uniform2fv(
      gl.getUniformLocation(program, "uTransformScaling"),
      Renderer.transformScale
    );

    gl.uniformMatrix4fv(
      gl.getUniformLocation(program, "u_matrix"),
      false,
      matrix
    );

    gl.enableVertexAttribArray(gl.getAttribLocation(program, "a_position")); // Enabling attributes locations for our shader to use.
    gl.vertexAttribPointer(
      gl.getAttribLocation(program, "a_position"),
      numOfComponents,
      type,
      normalize,
      stride,
      offset
    );

    offset = positionArray.length * 4;
    gl.enableVertexAttribArray(gl.getAttribLocation(program, "a_color"));
    gl.vertexAttribPointer(
      gl.getAttribLocation(program, "a_color"),
      numOfComponents,
      type,
      normalize,
      stride,
      offset
    );

    offset = 0;

    gl.drawArrays(mode, offset, NumVertices); // Drawing to canvas.
  }

  static populateColor(vColors, color, NumVertices) {
    for (var i = 0; i < NumVertices; i++) {
      vColors.push(new color4(color.r, color.g, color.b, color.a)); // Color values are driven from the TA'S Paper. They are as accurate as possible.
    }
  }

  static spiralMotionCalculator() {
    // Basically at every frame we rotate to the direction we are rotating (derived from our input box's number's sign), and..
    // ..we increase or decrease our distance from origin in the current direction we should. Derived from current state of..
    // ..animation and also can flip if we change the sign of input box's number.
    if (Renderer.ghostSpiralValue >= 1.0) {
      // If the sign of input box changed we check it here and flip our growth direction. This is since we will go backwards in animation.
      Renderer.ghostSpiralValue = 0;
      Renderer.currentDirectionGrowth *= -1;
    }
    if (Renderer.transformLength[0] >= 5 || Renderer.transformLength[1] >= 5) {
      // If we reach max distance from origin we start shrinking our distance.
      Renderer.currentDirectionGrowth = -1;
    }
    if (
      Renderer.currentDirectionGrowth === -1 &&
      (Renderer.transformLength[0] <= 0 || Renderer.transformLength[1] <= 0)
    ) {
      // If we were shrinking our distance and we are at origin we start increasing. Actually we don't need to check if we were shrinking since we are not going to reach 0 length from -1 length :)
      Renderer.currentDirectionGrowth = 1;
    }

    Renderer.transformCurrentAngle += 2.25 * Renderer.spiralValue; // Rotate our angle vector, it is calculated for 450 degrees to reach max distance. Also it speeds up with input value.

    if (Renderer.currentDirectionGrowth === 1) {
      // Derived for 450 degrees to reach max distance. We increment or decrement according to our current direction.
      Renderer.transformLength[0] += 0.025 * Math.abs(Renderer.spiralValue);
      Renderer.transformLength[1] += 0.025 * Math.abs(Renderer.spiralValue);
    } else {
      Renderer.transformLength[0] -= 0.025 * Math.abs(Renderer.spiralValue);
      Renderer.transformLength[1] -= 0.025 * Math.abs(Renderer.spiralValue);
    }

    Renderer.transformAngle =
      (Renderer.transformCurrentAngle * Math.PI) / 180.0; // We convert current angle to radian.

    Renderer.transformRotation[0] = Math.sin(Renderer.transformAngle); // We convert current radian angle to vector.
    Renderer.transformRotation[1] = Math.cos(Renderer.transformAngle);

    Renderer.transformPosition[0] =
      Renderer.transformRotation[0] * Renderer.transformLength[0]; // We multiply length vector and rotation vector to get transform position.
    Renderer.transformPosition[1] =
      Renderer.transformRotation[1] * Renderer.transformLength[1];
  }

  static _AnimationLoop() {
    // Animation loop.
    // With this method we can call it 60 times per second.
    requestAnimationFrame((t) => {
      let deltaAngle =
        ((t - Renderer.previousTime) / 1000.0) * Renderer.degreesPerSecond; // Converting time passed to delta angle to rotate.
      if (Renderer.spinState >= 1.0) {
        // Checking states and setting accordingly.
        Renderer.currentAngle =
          (Renderer.currentAngle + Renderer.spinValue * -1 * deltaAngle) % 360; // Calculating current angle from previous angle and delta angle and rotation direction.
      }
      if (Renderer.scaleState >= 1.0) {
        // Capping the scaling between 0.5 and 1.5
        if (Renderer.currentScale[0] >= 0.45) {
          // 0.3 is our default, scaled from experiment 1. so 0.3*1.5 is 0.45
          Renderer.scaleDirection = -1;
        }
        if (Renderer.currentScale[0] <= 0.15) {
          // 0.3 is our default, scaled from experiment 1. so 0.3*0.5 is 0.15
          Renderer.scaleDirection = 1;
        }
        // Considering our scale direction we scale the object.
        if (Renderer.scaleDirection === 1) {
          Renderer.currentScale[0] += 0.0025;
          Renderer.currentScale[1] += 0.0025;
        }
        if (Renderer.scaleDirection === -1) {
          Renderer.currentScale[0] -= 0.0025;
          Renderer.currentScale[1] -= 0.0025;
        }
      }
      // Spiral Animation is up above.
      if (Renderer.spiralState >= 1.0) {
        Renderer.spiralMotionCalculator();
      }
      Renderer.previousTime = t; // Setting previous time. We do this every frame so next frame we subtract this frame and get time passed.

      Renderer._AnimationLoop(); // Looping animation.
      Renderer.renderFrame(); // Rendering Frame.
    });
  }

  static renderFrame() {
    const gl = Renderer.GL;
    const program = Renderer.Program;
    const buffer = Renderer.Buffer;
    gl.clearColor(1, 1, 1, 1.0); // White background for the canvas.
    gl.clearDepth(1.0); // Clearing the canvas.
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    // Below are the components, obviously there are better ways to do it but for the sake of first experiment I tried to keep it simple.
    // NumVertices is determined to what to draw.
    // Other 2 variables are populated the position vertex array vPositions, the color array vColors.
    // After then simply rendering data.

    var NumVertices = 192; // High resolution for Big Yellow Circle because it is big and low resolution looks bad. Also this number is number of vertices and it will be get divided by 4 for resolution per Bezier Curve.
    var vPositions = [];
    var vColors = [];
    Circle.generate(vPositions, NumVertices / 4 - 1, 0, 0, 0, 2);
    // Circle function above or any other function at this point populates the vPositions, vertex array.
    Renderer.populateColor(
      vColors,
      new color4(0.93, 0.83, 0.09, 1.0),
      NumVertices
    ); // We manually populate the color array. Again this seems naive approach but I wanted to keep it simple
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors,
      [0.97, 0.6, 0.05, 1.0]
    ); // Rendering. Only the circle has a target color.

    NumVertices = 12; // Again we are doing the same procedure. First we declare the number of vertices that are going to create our shape. This determines resolution of the Bezier curves.
    vPositions = []; // Then we clear the vertex and color arrays.
    vColors = [];
    Bezier.generate(
      vPositions,
      NumVertices - 1,
      new point4(-1.25, 0, 0, 1),
      new point4(-0.5, 0.5, 0, 1),
      new point4(0.5, 0.5, 0, 1),
      new point4(1.25, 0, 0, 1)
    );
    // Populate vertices.
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    // Populate colors.
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );
    // And render, up above we just created the upper Bezier curve of the mask. Rendering done from back object to front object so the Big Circle was first and this features come after. Only if I had a scene renderer.

    NumVertices = 12; // We create the down curve of the mask.
    vPositions = [];
    vColors = [];
    Bezier.generate(
      vPositions,
      NumVertices - 1,
      new point4(-1.25, -1.25, 0, 1),
      new point4(-0.5, -1.75, 0, 1),
      new point4(0.5, -1.75, 0, 1),
      new point4(1.25, -1.25, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 4; // This is the big quad of the mask.
    vPositions = [];
    vColors = [];
    Quad.generate(
      vPositions,
      new point4(-1.25, 0, 0, 1),
      new point4(1.25, 0, 0, 1),
      new point4(1.25, -1.25, 0, 1),
      new point4(-1.25, -1.25, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 4; // This is left up string of the mask. I tried my best to match the shape and thickness.
    vPositions = [];
    vColors = [];
    Quad.generate(
      vPositions,
      new point4(-1.25, 0.0, 0, 1),
      new point4(-1.98, 0.25, 0, 1),
      new point4(-2, 0.05, 0, 1),
      new point4(-1.25, -0.2, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 12; // We also add bezier curves the end of the strings as TA stated. This makes it look more like the picture proven. However they are slight curves. I did my best to match the paper given.
    vPositions = [];
    vColors = [];
    Bezier.generate(
      vPositions,
      NumVertices - 1,
      new point4(-1.98, 0.25, 0, 1),
      new point4(-2.0, 0.2, 0, 1),
      new point4(-2.0, 0.1, 0, 1),
      new point4(-2, 0.05, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 4; // Upper right string of the mask.
    vPositions = [];
    vColors = [];
    Quad.generate(
      vPositions,
      new point4(1.25, 0.0, 0, 1),
      new point4(1.98, 0.25, 0, 1),
      new point4(2, 0.05, 0, 1),
      new point4(1.25, -0.2, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 12; // Curve of upper right string.
    vPositions = [];
    vColors = [];
    Bezier.generate(
      vPositions,
      NumVertices - 1,
      new point4(1.98, 0.25, 0, 1),
      new point4(2.0, 0.2, 0, 1),
      new point4(2.0, 0.1, 0, 1),
      new point4(2, 0.05, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 4; // Right down string of the mask.
    vPositions = [];
    vColors = [];
    Quad.generate(
      vPositions,
      new point4(1.1, -1.01, 0, 1),
      new point4(1.59, -1.22, 0, 1),
      new point4(1.49, -1.34, 0, 1),
      new point4(1.1, -1.2, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 12; // Curve of the right down string.
    vPositions = [];
    vColors = [];
    Bezier.generate(
      vPositions,
      NumVertices - 1,
      new point4(1.59, -1.22, 0, 1),
      new point4(1.56, -1.27, 0, 1),
      new point4(1.51, -1.32, 0, 1),
      new point4(1.49, -1.34, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 4; // Left down string of the mask.
    vPositions = [];
    vColors = [];
    Quad.generate(
      vPositions,
      new point4(-1.1, -1.01, 0, 1),
      new point4(-1.59, -1.22, 0, 1),
      new point4(-1.49, -1.34, 0, 1),
      new point4(-1.1, -1.2, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 12; // Curve of the left down string.
    vPositions = [];
    vColors = [];
    Bezier.generate(
      vPositions,
      NumVertices - 1,
      new point4(-1.59, -1.22, 0, 1),
      new point4(-1.56, -1.27, 0, 1),
      new point4(-1.51, -1.32, 0, 1),
      new point4(-1.49, -1.34, 0, 1)
    );
    Renderer.populateColor(
      vColors,
      new color4(0.84, 0.88, 0.92, 1.0),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 48; // The eyes.This one is the left eye.
    vPositions = [];
    vColors = [];
    Circle.generate(vPositions, NumVertices / 4 - 1, -0.64, 0.8, 0, 0.3);
    Renderer.populateColor(
      vColors,
      new color4(0.23, 0.21, 0.08, 1),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );

    NumVertices = 48; // Right eye.
    vPositions = [];
    vColors = [];
    Circle.generate(vPositions, NumVertices / 4 - 1, 0.64, 0.8, 0, 0.3);
    Renderer.populateColor(
      vColors,
      new color4(0.23, 0.21, 0.08, 1),
      NumVertices
    );
    Renderer.render(
      gl,
      program,
      gl.TRIANGLE_FAN,
      NumVertices,
      vPositions,
      vColors
    );
  }
}

function startSpin() {
  Renderer.spinState = 1;
}

function stopSpin() {
  Renderer.spinState = 0;
}

function spinInputHandler() {
  Renderer.spinValue = document.getElementById("spinInput").value;
}

function startScale() {
  Renderer.scaleState = 1;
}

function stopScale() {
  Renderer.scaleState = 0;
}

function startSpiral() {
  Renderer.spiralState = 1;
}

function stopSpiral() {
  Renderer.spiralState = 0;
}

function spiralInputHandler() {
  if (Renderer.spiralValue * document.getElementById("spiralInput").value < 0) {
    // We multiply current value with previous if they are same we get positive if not we detect input sign flip.
    // We will catch it in animation frame and make sure to flip our direction of growth.
    Renderer.ghostSpiralValue = 1.0;
  }
  Renderer.spiralValue = document.getElementById("spiralInput").value;
}

window.addEventListener("DOMContentLoaded", () => {
  // After the content of html loaded we call the main function. This helps with resolution problems and other problems such as styling etc.
  Renderer._Init();
});
