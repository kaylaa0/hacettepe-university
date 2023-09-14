/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

class Shader {
  constructor(gl, vertShaderSrc, fragShaderSrc) {
    this.program = ShaderUtil.createProgramFromText(
      gl,
      vertShaderSrc,
      fragShaderSrc,
      true
    );

    if (this.program != null) {
      this.gl = gl;
      gl.useProgram(this.program);
      this.attribLoc = ShaderUtil.getStandardAttribLocations(gl, this.program);
      this.uniformLoc = ShaderUtil.getStandardUniformLocations(
        gl,
        this.program
      );
    }
  }

  //Methods
  activate() {
    this.gl.useProgram(this.program);
    return this;
  }
  deactivate() {
    this.gl.useProgram(null);
    return this;
  }

  setPerspective(matData) {
    this.gl.uniformMatrix4fv(this.uniformLoc.perspective, false, matData);
    return this;
  }
  setModalMatrix(matData) {
    this.gl.uniformMatrix4fv(this.uniformLoc.modalMatrix, false, matData);
    return this;
  }
  setCameraMatrix(matData) {
    this.gl.uniformMatrix4fv(this.uniformLoc.cameraMatrix, false, matData);
    return this;
  }

  //Setup custom properties
  preRender() {} //abstract method, extended object may need need to do some things before rendering.

  //Handle rendering a modal
  renderModal(modal) {
    this.setModalMatrix(modal.transform.getViewMatrix()); //Set the transform, so the shader knows where the modal exists in 3d space
    this.gl.bindVertexArray(modal.mesh.vao); //Enable VAO, this will set all the predefined attributes for the shader

    if (modal.mesh.noCulling) this.gl.disable(this.gl.CULL_FACE);
    if (modal.mesh.doBlending) this.gl.enable(this.gl.BLEND);

    if (modal.mesh.indexCount)
      this.gl.drawElements(
        modal.mesh.drawMode,
        modal.mesh.indexCount,
        gl.UNSIGNED_SHORT,
        0
      );
    else this.gl.drawArrays(modal.mesh.drawMode, 0, modal.mesh.vertexCount);

    //Cleanup
    this.gl.bindVertexArray(null);
    if (modal.mesh.noCulling) this.gl.enable(this.gl.CULL_FACE);
    if (modal.mesh.doBlending) this.gl.disable(this.gl.BLEND);
    return this;
  }
}

class ShaderUtil {
  //Create a shader by passing in its code and what type
  static createShader(gl, src, type) {
    var shader = gl.createShader(type);
    gl.shaderSource(shader, src);
    gl.compileShader(shader);

    //Get Error data if shader failed compiling
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      console.error(
        "Error compiling shader : " + src,
        gl.getShaderInfoLog(shader)
      );
      gl.deleteShader(shader);
      return null;
    }

    return shader;
  }

  //Link two compiled shaders to create a program for rendering.
  static createProgram(gl, vShader, fShader, doValidate) {
    //Link shaders together
    var prog = gl.createProgram();
    gl.attachShader(prog, vShader);
    gl.attachShader(prog, fShader);

    //Force predefined locations for specific attributes. If the attibute isn't used in the shader its location will default to -1
    gl.bindAttribLocation(prog, ATTR_POSITION_LOC, ATTR_POSITION_NAME);
    gl.bindAttribLocation(prog, ATTR_NORMAL_LOC, ATTR_NORMAL_NAME);
    gl.bindAttribLocation(prog, ATTR_UV_LOC, ATTR_UV_NAME);

    gl.linkProgram(prog);

    //Check if successful
    if (!gl.getProgramParameter(prog, gl.LINK_STATUS)) {
      console.error(
        "Error creating shader program.",
        gl.getProgramInfoLog(prog)
      );
      gl.deleteProgram(prog);
      return null;
    }

    //Only do this for additional debugging.
    if (doValidate) {
      gl.validateProgram(prog);
      if (!gl.getProgramParameter(prog, gl.VALIDATE_STATUS)) {
        console.error("Error validating program", gl.getProgramInfoLog(prog));
        gl.deleteProgram(prog);
        return null;
      }
    }

    //Can delete the shaders since the program has been made.
    gl.detachShader(prog, vShader); //TODO, detaching might cause issues on some browsers, Might only need to delete.
    gl.detachShader(prog, fShader);
    gl.deleteShader(fShader);
    gl.deleteShader(vShader);

    return prog;
  }

  //Pass in Script Tag IDs for our two shaders and create a program from it.

  static createProgramFromText(gl, vShaderTxt, fShaderTxt, doValidate) {
    var vShader = ShaderUtil.createShader(gl, vShaderTxt, gl.VERTEX_SHADER);
    if (!vShader) return null;
    var fShader = ShaderUtil.createShader(gl, fShaderTxt, gl.FRAGMENT_SHADER);
    if (!fShader) {
      gl.deleteShader(vShader);
      return null;
    }

    return ShaderUtil.createProgram(gl, vShader, fShader, true);
  }

  //Get the locations of standard Attributes that we will mostly be using. Location will = -1 if attribute is not found.
  static getStandardAttribLocations(gl, program) {
    return {
      position: gl.getAttribLocation(program, ATTR_POSITION_NAME),
      norm: gl.getAttribLocation(program, ATTR_NORMAL_NAME),
      uv: gl.getAttribLocation(program, ATTR_UV_NAME),
    };
  }

  static getStandardUniformLocations(gl, program) {
    return {
      perspective: gl.getUniformLocation(program, "uPMatrix"),
      modalMatrix: gl.getUniformLocation(program, "uMVMatrix"),
      cameraMatrix: gl.getUniformLocation(program, "uCameraMatrix"),
      mainTexture: gl.getUniformLocation(program, "uMainTex"),
    };
  }
}
