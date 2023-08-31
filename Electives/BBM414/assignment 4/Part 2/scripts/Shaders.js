/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

const ShaderDatabase = {
  vShader: `#version 300 es
        in vec3 a_position;	//Making it a vec4, the w component is used as color index from uColor
        in vec3 a_norm;
        
        uniform vec3 a_color;
        uniform mat4 uPMatrix;
        uniform mat4 uMVMatrix;
        uniform mat4 uCameraMatrix;
        
        out vec4 color;
        out vec3 vLighting;
        
        void main(void){
            gl_Position = uPMatrix * uCameraMatrix * uMVMatrix * vec4(a_position.xyz, 1.0);
            
            vec3 ambientLight = vec3(0.3, 0.3, 0.3);
            vec3 directionalLightColor = vec3(1, 1, 1);
            vec3 directionalVector = normalize(vec3(0.85, 0.8, 0.75));
            
            mat4 uNormalMatrix = transpose(inverse(uMVMatrix));
            
            vec4 transformedNormal = uNormalMatrix * vec4(a_norm, 1.0);
            
            float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
            vLighting = ambientLight + (directionalLightColor * directional);
            
            color = vec4(a_color, 1.0);
        }`,
  fShader: `#version 300 es
        precision mediump float;
        
        in vec3 vLighting;
        
        in vec4 color;
        
        out vec4 finalColor;
        void main(void){
            finalColor = vec4(color.rgb * vLighting, color.a);
        }`,
  vShaderTextured: `#version 300 es
        in vec4 a_position;
        in vec2 a_uv;
    
        uniform mat4 uPMatrix;
        uniform mat4 uMVMatrix;
        uniform mat4 uCameraMatrix;
    
        out highp vec3 texCoord;  //Interpolate UV values to the fragment shader
    
        void main(void){
            texCoord = a_position.xyz;
            gl_Position = uPMatrix * uCameraMatrix * vec4(a_position.xyz, 1.0);
        }`,
  fShaderTextured: `#version 300 es
        precision mediump float;
    
        in highp vec3 texCoord;
        uniform samplerCube uDayTex;
    
        out vec4 finalColor;
        void main(void){
            finalColor = texture(uDayTex, texCoord);
        }`,
};

class DefaultShader extends Shader {
  constructor(gl, pMatrix) {
    var vertSrc = ShaderDatabase["vShader"],
      fragSrc = ShaderDatabase["fShader"];

    super(gl, vertSrc, fragSrc);

    //Custom Uniforms
    this.uniformLoc.a_color = gl.getUniformLocation(this.program, "a_color");

    //Standard Uniforms
    this.setPerspective(pMatrix);
    this.mainTexture = -1; //Store Our Texture ID
    gl.useProgram(null); //Done setting up shader
  }

  setTexture(texID) {
    this.mainTexture = texID;
    return this;
  }

  //Override
  preRender(ShaderVariables) {
    //Setup Texture
    this.gl.uniform3fv(
      this.uniformLoc.a_color,
      ShaderVariables["colorOverRide"]
    );
    return this;
  }
}

class SkyBoxShader extends Shader {
  constructor(gl, pMatrix, dayTex) {
    var vertSrc = ShaderDatabase["vShaderTextured"],
      fragSrc = ShaderDatabase["fShaderTextured"];
    super(gl, vertSrc, fragSrc);

    //Custom Uniforms
    this.uniformLoc.dayTex = gl.getUniformLocation(this.program, "uDayTex");

    //Standard Uniforms
    this.setPerspective(pMatrix);
    this.texDay = dayTex;
    gl.useProgram(null); //Done setting up shader
  }

  //Override
  preRender(ShaderVariables) {
    //Setup Texture
    this.gl.activeTexture(this.gl.TEXTURE0);
    this.gl.bindTexture(this.gl.TEXTURE_CUBE_MAP, this.texDay);
    this.gl.uniform1i(this.uniformLoc.dayTex, 0);

    return this;
  }
}
