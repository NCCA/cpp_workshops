#version 420                                            // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable

// Attribute passed from the vertex shader
smooth in vec3 o_VertexColour;

// The texture to be mapped
uniform sampler2D u_Texture;

// This is no longer a built-in variable
out vec4 o_FragColor;

void main() {
    // Set the output color of our current pixel
    o_FragColor = vec4(o_VertexColour, 1.0);
}
