"use strict";
/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Bezier {
  // Main Bezier function based on sources from web.
  static generate(out, res, p0, p1, p2, p3) {
    // Inputs, out = the points/vertex data array, res = Bezier function resolution higher the resolution more lines to create curve, p0 and p3 are points to create curve in between, p1 and p2 are the weight points.
    var inc = 1 / res, // Increment depends on resolution, higher resolution means lower increment, means more lines for curve.
      pos = new Vec3(); // Temp vector to hold our current position along curve.
    for (var i = 0; i < res; i++) {
      Bezier.getPoint(p0, p1, p2, p3, i * inc, pos); // Gets current calculated point along Bezier Curve.
      out.push(new point4(pos.x, pos.y, pos.z, 1)); // Puts it in output array.
    }
    Bezier.getPoint(p0, p1, p2, p3, 1, pos); // Lastly puts the last point in the array too. Last point is also the p3.
    out.push(new point4(pos.x, pos.y, pos.z, 1));
  }

  static getPoint(p0, p1, p2, p3, t, out) {
    if (t > 1) t = 1;
    else if (t < 0) t = 0;

    var i = 1 - t;
    // Specified Bezier Function.
    out = out || new Vec3();
    out[0] =
      i * i * i * p0.x +
      3 * i * i * t * p1.x +
      3 * i * t * t * p2.x +
      t * t * t * p3.x;
    out[1] =
      i * i * i * p0.y +
      3 * i * i * t * p1.y +
      3 * i * t * t * p2.y +
      t * t * t * p3.y;
    out[2] =
      i * i * i * p0.z +
      3 * i * i * t * p1.z +
      3 * i * t * t * p2.z +
      t * t * t * p3.z;
    return out;
  }
}

class Circle {
  // Circle primitive to create circles.
  static generate(out, res, pX, pY, pZ, r) {
    // Takes input, out = vertex array, res = resolution of bezier curves of the circle, pX and pY and pZ are the coordinates of the center of the circle, r is the radius of circle.
    var top = new point4(pX, pY + r, pZ, 1), // First calculating top right left and bottom points by simply calculating center + radius.
      right = new point4(pX + r, pY, pZ, 1),
      left = new point4(pX - r, pY, pZ, 1),
      bottom = new point4(pX, pY - r, pZ, 1);
    // We generate 4 curves per circle. Going clockwise.
    Bezier.generate(
      out,
      res,
      top,
      this.getPerfectWeight(top, new Vec3().set(r, 0, 0)),
      this.getPerfectWeight(right, new Vec3().set(0, r, 0)),
      right
    );
    Bezier.generate(
      out,
      res,
      right,
      this.getPerfectWeight(right, new Vec3().set(0, -r, 0)),
      this.getPerfectWeight(bottom, new Vec3().set(r, 0, 0)),
      bottom
    );
    Bezier.generate(
      out,
      res,
      bottom,
      this.getPerfectWeight(bottom, new Vec3().set(-r, 0, 0)),
      this.getPerfectWeight(left, new Vec3().set(0, -r, 0)),
      left
    );
    Bezier.generate(
      out,
      res,
      left,
      this.getPerfectWeight(left, new Vec3().set(0, r, 0)),
      this.getPerfectWeight(top, new Vec3().set(-r, 0, 0)),
      top
    );
  }

  static getPerfectWeight(point, direction) {
    // This function is used to determine weight points of the bezier curve. The constant below comes from (4/3)*tan(pi/(2n)).
    direction.multi(0.552284749831);
    return new point4(
      point.x + direction.x,
      point.y + direction.y,
      point.z + direction.z,
      point.alpha
    );
  }
}

class Quad {
  // Basic Quad generator. It does not create 2 triangles instead creates a basic Quad directly.
  static generate(vPositions, a, b, c, d) {
    // It takes output array and 4 corners of the quad.
    var Index = 0;
    vPositions[Index] = a;
    Index++;
    vPositions[Index] = b;
    Index++;
    vPositions[Index] = c;
    Index++;
    vPositions[Index] = d;
    Index++;
  }
}

class Renderer {
  // These are used in renderer
  static GL = null;
  static Program = null;
  static Buffer = null;
  static currentAngle = 0.0;
  static currentRotation = [0, 1];

  static currentScale = [0.4, 0.4]; // You can change this manually to scale.

  static previousTime = 0.0;
  static degreesPerSecond = 45.0;

  static state = 1; // State of program
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

    if (colorTo === undefined) {
      // If color to is not defined, that means current render has no color shift so we do accordingly.
      colorTo = [0.0, 0.0, 0.0, 0.0];
      let temp = 0.0;
      gl.uniform1f(gl.getUniformLocation(program, "mixAngle"), temp);
    } else {
      gl.uniform1f(
        gl.getUniformLocation(program, "mixAngle"),
        Renderer.currentAngle
      );
    }

    gl.uniform1f(gl.getUniformLocation(program, "state"), Renderer.state);
    gl.uniform4fv(gl.getUniformLocation(program, "a_color_to"), colorTo);

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

  static _AnimationLoop() {
    // Animation loop.
    // With this method we can call it 60 times per second.
    requestAnimationFrame((t) => {
      let deltaAngle =
        ((t - Renderer.previousTime) / 1000.0) * Renderer.degreesPerSecond; // Converting time passed to delta angle to rotate.
      if (Renderer.state === 1.0) {
        // Checking states and setting accordingly.
        Renderer.currentAngle = 0; // State 1 resets variables.
        Renderer.rotationDirection = -1;
      } else if (Renderer.state >= 2.0) {
        if (Renderer.currentAngle <= -45) {
          // Angle clipping the rotation.
          Renderer.rotationDirection = 1;
        } else if (Renderer.currentAngle >= 45) {
          Renderer.rotationDirection = -1;
        }
        Renderer.currentAngle =
          (Renderer.currentAngle + Renderer.rotationDirection * deltaAngle) %
          360; // Calculating current angle from previous angle and delta angle and rotation direction.
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

window.addEventListener("DOMContentLoaded", () => {
  // After the content of html loaded we call the main function. This helps with resolution problems and other problems such as styling etc.
  Renderer._Init();
  document.addEventListener("keydown", function (event) {
    // Hooking keyboard inputs to change state of renderer.
    if (event.code === "Digit1") {
      Renderer.state = 1;
    } else if (event.code === "Numpad1") {
      Renderer.state = 1;
    } else if (event.code === "Digit2") {
      Renderer.state = 2;
    } else if (event.code === "Numpad2") {
      Renderer.state = 2;
    } else if (event.code === "Digit3") {
      Renderer.state = 3;
    } else if (event.code === "Numpad3") {
      Renderer.state = 3;
    }
  });
});
