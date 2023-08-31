/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

// Vertex shader multiples vertices with the matrix meaning fitting them in the view.
// Our matrix is very basic without camera rotations etc. so this is basic.
// Also assigns the color to vertex.

const vShader = `#version 300 es
    in vec4 a_position;
    in vec4 a_color;
    uniform mat4 u_matrix;
    out vec4 color;
    
    void main() {
        gl_Position = u_matrix * a_position;
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
