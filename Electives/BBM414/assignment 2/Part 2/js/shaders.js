/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

const vShader = `#version 300 es
    in vec2 a_position;
    in vec4 a_color;
    
    uniform vec4 a_color_to;
    uniform mat4 u_matrix;
    uniform float state;
    
    uniform vec2 uScalingFactor;
    uniform vec2 uRotationVector;
    uniform float mixAngle;
    
    out vec4 color;
    out vec4 color_to;
    out float mix_wave;
   
    void main() {
        if(state >= 2.0){
            vec2 rotatedPosition = vec2(
              a_position.x * uRotationVector.y +
                    a_position.y * uRotationVector.x,
              a_position.y * uRotationVector.y -
                    a_position.x * uRotationVector.x
            );
            gl_Position = u_matrix * vec4(rotatedPosition * uScalingFactor, 0.0, 1.0);
           if(state >= 3.0){
            mix_wave = mixAngle;
           }
        }else{
            gl_Position = u_matrix * vec4(a_position * uScalingFactor, 0.0, 1.0);
            mix_wave = 0.0;
        }
        
        color = a_color;
        color_to = a_color_to;
    }
`;

const fShader = `#version 300 es
// fragment shaders don't have a default precision so we need
// to pick one. mediump is a good default. It means "medium precision"
    precision mediump float;
    
    in vec4 color;
    in vec4 color_to;
    in float mix_wave;
    
    out vec4 outColor;
    

    void main() {
        outColor = mix(color, color_to, abs(mix_wave)/45.0);
    }
`;
