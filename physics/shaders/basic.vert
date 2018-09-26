#version 420                                            // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable
//#extension GL_ARB_shading_language_420pack: enable    // Use for GLSL versions before 420.


// The modelview and projection matrices are no longer given in OpenGL 4.2
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix; // This is the inverse transpose of the mv matrix

// The vertex position attribute
in vec3 a_VertexPosition;

// The vertex colour attribute
in vec3 a_VertexColour;

// The vertex colour attribute
smooth out vec3 o_VertexColour;

void main() {
    // Set the position of the current vertex
    vec4 vpos = u_ModelMatrix * vec4(a_VertexPosition,1.0);
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vpos;
    o_VertexColour = a_VertexColour;
}
