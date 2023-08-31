/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

const vShader = `#version 300 es
    in vec2 a_position;
    in vec4 a_color;
    
    uniform mat4 u_matrix;
    
    uniform vec2 uScalingFactor;
    uniform vec2 uRotationVector;
    
    uniform vec2 uTransformPosition;
    uniform vec2 uTransformScaling;
    
    out vec4 color;
   
    void main() {
        vec2 rotatedPosition = vec2(
              a_position.x * uRotationVector.y +
                    a_position.y * uRotationVector.x,
              a_position.y * uRotationVector.y -
                    a_position.x * uRotationVector.x
            );
        gl_Position = u_matrix * vec4(rotatedPosition * uScalingFactor, 0.0, 1.0) + vec4(uTransformPosition*uTransformScaling, 0.0, 0.0);
        
        color = a_color;
    }
`;

const fShader = `#version 300 es
// fragment shaders don't have a default precision so we need
// to pick one. mediump is a good default. It means "medium precision"
    precision mediump float;
    
    in vec4 color;
    
    out vec4 outColor;
    
    void main() {
        outColor = color;
    }
`;
