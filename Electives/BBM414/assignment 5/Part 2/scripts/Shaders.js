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
            
            vec3 ambientLight = vec3(1.0, 1.0, 1.0);
            vec3 directionalLightColor = vec3(1, 1, 1);
            vec3 directionalVector = normalize(vec3(0.85, 0.8, 0.75));
            
            mat4 uNormalMatrix = transpose(inverse(uMVMatrix));
            
            vec4 transformedNormal = uNormalMatrix * vec4(a_norm, 1.0);
            
            float directional = max(dot(transformedNormal.xyz, directionalVector), 0.0);
            vLighting = ambientLight * (directionalLightColor * directional);
            
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
  vShaderTexturedAndVertexSpotLight: `#version 300 es
        in vec4 a_position;
        in vec3 a_norm;
        in vec2 a_uv;
       
        
        uniform vec4 ambientProduct, diffuseProduct, specularProduct;
        uniform mat4 uMVMatrix;
        uniform mat4 uPMatrix;
        uniform mat4 uCameraMatrix;
        uniform mat4 uCameraModalMatrix;
        uniform vec4 u_Light_position;
        uniform vec4 u_Light_direction;
        uniform float u_Shininess;
        uniform float toggleOff;
                
        out vec4 fColor;
        out highp vec2 fTexCoord;
        
        void main()
        {
        
            vec3 pos = (uMVMatrix * a_position).xyz;
        
            //fixed light postion
        
            vec3 light = u_Light_position.xyz;
            
            vec3 L = normalize( light - pos );
            vec3 SD = normalize(u_Light_direction.xyz);
            
            vec3 E = normalize( -pos );
            vec3 H = normalize( L + E );
        
            vec4 NN = vec4(a_norm,0);
        
            // Transform vertex normal into eye coordinates
        
            vec3 N = normalize( (uMVMatrix*NN).xyz);
        
            // Compute terms in the illumination equation
            vec4 ambient = ambientProduct;
        
            float Kd = max( dot(L, N), 0.0 );
            vec4  diffuse = Kd*diffuseProduct;
        
            float Ks = pow( max(dot(N, H), 0.0), u_Shininess );
            vec4  specular = Ks * specularProduct;
        
            if( dot(L, N) < 0.0 ) {
                specular = vec4(0.0, 0.0, 0.0, 1.0);
            }
        
            gl_Position = uPMatrix * uCameraMatrix * uMVMatrix * vec4(a_position.xyz,1.0);
            if(dot(L, SD) > 0.95){
                fColor = diffuse;
            }else{
                fColor = ambient;
            }
            if(toggleOff >= 1.0){
                fColor = ambient;
            }
        
            fColor.a = 1.0;
            fTexCoord = a_uv;
        }
    `,
  fShaderTexturedAndVertexSpotLight: `#version 300 es

        precision mediump float;
        
        
        in vec4 fColor;
        in highp vec2 fTexCoord;

        uniform sampler2D Texture;
        
        out vec4 oColor;
        
        void
        main()
        {
            oColor = fColor * texture( Texture, fTexCoord );
        }
    `,
  vShaderTexturedAndFragmentSpotLight: `#version 300 es
        in vec4 a_position;
        in vec3 a_norm;
        in vec2 a_uv;

        uniform mat4 uPMatrix;
        uniform mat4 uMVMatrix;
        uniform mat4 uCameraMatrix;

        uniform vec4 u_Light_position;
        uniform vec3 lightVector;

        out mediump vec3 fN;
        out mediump vec3 fV;
        out mediump vec3 fL;

        out highp vec2 texCoord; //Interpolate UV values to the fragment shader

        void main(void){
            fN = (uMVMatrix*vec4(a_norm,0)).xyz;
            fV = (uMVMatrix*a_position).xyz;
            fL = normalize( vec3(u_Light_position.xyz) - vec3((uMVMatrix * a_position).xyz) );
            
            texCoord = a_uv;
            gl_Position = uPMatrix * uCameraMatrix * uMVMatrix  * vec4(a_position.xyz, 1.0);
  
        }`,
  fShaderTexturedAndFragmentSpotLight: `#version 300 es
        precision mediump float;

        
        in mediump vec3 fN;
        in mediump vec3 fV;
        in mediump vec3 fL;
        
        in highp vec2 texCoord;
        
        uniform vec4 ambientProduct, diffuseProduct, specularProduct;
        
        uniform vec4 u_Light_direction;
      
        uniform float u_Shininess;

        uniform float toggleOff;
        
        uniform sampler2D Texture;

        out vec4 finalColor;
        void main(void){
            vec3 N = normalize(fN);
            vec3 V = normalize(fV);
            vec3 L = normalize(fL) ;
            vec3 H = normalize( L + V ); 
            vec3 SD = normalize(u_Light_direction.xyz);
            vec4 ambient = ambientProduct;
            
            float Kd = max(dot(L, N), 0.0);
            vec4 diffuse = Kd*diffuseProduct;
            float Ks = pow(max(dot(N, H), 0.0), u_Shininess);
            vec4 specular = Ks*specularProduct;
            // discard the specular highlight if the light's behind the vertex
            if( dot(L, N) < 0.0 ) {
                specular = vec4(0.0, 0.0, 0.0, 1.0);
            }
            if(dot(SD, L) > 0.95){
                finalColor = (diffuse) *  texture(Texture, texCoord);
            }else{
                finalColor = ambient;
            }
            if(toggleOff >= 1.0){
                finalColor = ambient;
            }
            finalColor.a = 1.0;
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

class TextureLightFragmentShader extends Shader {
  constructor(gl, pMatrix, textureImage) {
    var vertSrc = ShaderDatabase["vShaderTexturedAndFragmentSpotLight"],
      fragSrc = ShaderDatabase["fShaderTexturedAndFragmentSpotLight"];
    super(gl, vertSrc, fragSrc);

    //Custom Uniforms
    this.uniformLoc.textureImage = gl.getUniformLocation(
      this.program,
      "Texture"
    );
    this.uniformLoc.lightPosition = gl.getUniformLocation(
      this.program,
      "u_Light_position"
    );
    this.uniformLoc.lightDirection = gl.getUniformLocation(
      this.program,
      "u_Light_direction"
    );
    this.uniformLoc.lightColor = gl.getUniformLocation(
      this.program,
      "u_Light_color"
    );
    this.uniformLoc.shininess = gl.getUniformLocation(
      this.program,
      "u_Shininess"
    );
    this.uniformLoc.ambientProduct = gl.getUniformLocation(
      this.program,
      "ambientProduct"
    );
    this.uniformLoc.diffuseProduct = gl.getUniformLocation(
      this.program,
      "diffuseProduct"
    );
    this.uniformLoc.specularProduct = gl.getUniformLocation(
      this.program,
      "specularProduct"
    );
    this.uniformLoc.toggleOff = gl.getUniformLocation(
      this.program,
      "toggleOff"
    );

    //Standard Uniforms
    this.setPerspective(pMatrix);
    this.textureImage = textureImage;
    gl.useProgram(null); //Done setting up shader
  }

  //Override
  preRender(ShaderVariables) {
    //Setup Texture
    //this.gl.activeTexture(this.gl.TEXTURE0);
    this.gl.bindTexture(this.gl.TEXTURE_2D, this.textureImage);
    this.gl.uniform1i(this.uniformLoc.textureImage, 0);
    this.gl.uniform4fv(this.uniformLoc.lightPosition, [
      KaylaEngine.camera.transform.position.x,
      KaylaEngine.camera.transform.position.y,
      KaylaEngine.camera.transform.position.z,
      1.0,
    ]);
    this.gl.uniform4fv(this.uniformLoc.lightDirection, [
      KaylaEngine.camera.cameraMatrix[8],
      KaylaEngine.camera.cameraMatrix[9],
      KaylaEngine.camera.cameraMatrix[10],
      0.0,
    ]);
    this.gl.uniform4fv(this.uniformLoc.ambientProduct, [0.0, 0.0, 0.0, 0.0]);
    this.gl.uniform4fv(this.uniformLoc.diffuseProduct, [1.0, 1.0, 1.0, 0.0]);
    this.gl.uniform4fv(this.uniformLoc.specularProduct, [0.1, 0.1, 0.1, 0.0]);
    this.gl.uniform4fv(this.uniformLoc.lightColor, [1.0, 1.0, 1.0, 1.0]);
    this.gl.uniform1f(this.uniformLoc.shininess, 0.5);
    this.gl.uniform1f(this.uniformLoc.toggleOff, KaylaEngine.toggleOff);

    return this;
  }
}

class TextureLightShader extends Shader {
  constructor(gl, pMatrix, textureImage) {
    var vertSrc = ShaderDatabase["vShaderTexturedAndVertexSpotLight"],
      fragSrc = ShaderDatabase["fShaderTexturedAndVertexSpotLight"];
    super(gl, vertSrc, fragSrc);

    //Custom Uniforms
    this.uniformLoc.textureImage = gl.getUniformLocation(
      this.program,
      "Texture"
    );
    this.uniformLoc.lightPosition = gl.getUniformLocation(
      this.program,
      "u_Light_position"
    );

    this.uniformLoc.lightDirection = gl.getUniformLocation(
      this.program,
      "u_Light_direction"
    );
    this.uniformLoc.cameraModalMatrix = gl.getUniformLocation(
      this.program,
      "uCameraModalMatrix"
    );
    this.uniformLoc.lightColor = gl.getUniformLocation(
      this.program,
      "u_Light_color"
    );
    this.uniformLoc.shininess = gl.getUniformLocation(
      this.program,
      "u_Shininess"
    );
    this.uniformLoc.ambientProduct = gl.getUniformLocation(
      this.program,
      "ambientProduct"
    );
    this.uniformLoc.diffuseProduct = gl.getUniformLocation(
      this.program,
      "diffuseProduct"
    );
    this.uniformLoc.specularProduct = gl.getUniformLocation(
      this.program,
      "specularProduct"
    );
    this.uniformLoc.toggleOff = gl.getUniformLocation(
      this.program,
      "toggleOff"
    );

    //Standard Uniforms
    this.setPerspective(pMatrix);
    this.textureImage = textureImage;
    gl.useProgram(null); //Done setting up shader
  }

  //Override
  preRender(ShaderVariables) {
    //Setup Texture
    //this.gl.activeTexture(this.gl.TEXTURE0);
    this.gl.bindTexture(this.gl.TEXTURE_2D, this.textureImage);
    this.gl.uniform1i(this.uniformLoc.textureImage, 0);

    this.gl.uniformMatrix4fv(
      this.uniformLoc.cameraModalMatrix,
      false,
      KaylaEngine.camera.transform.getViewMatrix()
    );
    //this.gl.uniform4fv(this.uniformLoc.lightPosition,[KaylaEngine.camera.viewMatrix.raw[12], -KaylaEngine.camera.viewMatrix.raw[13], KaylaEngine.camera.viewMatrix.raw[14],1.0]);
    this.gl.uniform4fv(this.uniformLoc.lightPosition, [
      KaylaEngine.camera.transform.position.x,
      KaylaEngine.camera.transform.position.y,
      KaylaEngine.camera.transform.position.z,
      1.0,
    ]);
    this.gl.uniform4fv(this.uniformLoc.lightDirection, [
      KaylaEngine.camera.cameraMatrix[8],
      KaylaEngine.camera.cameraMatrix[9],
      KaylaEngine.camera.cameraMatrix[10],
      0.0,
    ]);
    this.gl.uniform4fv(this.uniformLoc.ambientProduct, [0.0, 0.0, 0.0, 0.0]);
    this.gl.uniform4fv(this.uniformLoc.diffuseProduct, [1.0, 1.0, 1.0, 0.0]);
    this.gl.uniform4fv(this.uniformLoc.specularProduct, [0.1, 0.1, 0.1, 0.0]);
    this.gl.uniform4fv(this.uniformLoc.lightColor, [1.0, 1.0, 1.0, 1.0]);
    this.gl.uniform1f(this.uniformLoc.shininess, 0.5);
    this.gl.uniform1f(this.uniformLoc.toggleOff, KaylaEngine.toggleOff);

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
