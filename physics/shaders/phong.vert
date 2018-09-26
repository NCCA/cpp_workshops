#version 420                                            // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable
//#extension GL_ARB_shading_language_420pack: enable    // Use for GLSL versions before 420.

// The modelview and projection matrices are no longer given in OpenGL 4.2
uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat3 u_NormalMatrix;

// The vertex position attribute
in vec3 a_VertexPosition;

// The vertex normal attribute
in vec3 a_VertexNormal;

// The texture coordinate attribute
in vec2 a_TexCoord;

// These attributes are passed onto the shader (should they all be smoothed?)
smooth out vec3 o_VertexPosition;
smooth out vec3 o_VertexNormal;
smooth out vec2 o_TexCoord;

void main() {
    // Transform the vertex normal by the inverse transpose modelview matrix
    o_VertexNormal = normalize(u_NormalMatrix * normalize(a_VertexNormal));

    // Compute the unprojected vertex position
    vec4 vpos = u_ModelMatrix * vec4(a_VertexPosition,1.0);
    o_VertexPosition = vec3(vpos) / vpos.w;

    // Copy across the texture coordinates
    o_TexCoord = a_TexCoord;

    // Compute the position of the vertex
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vpos;
}
