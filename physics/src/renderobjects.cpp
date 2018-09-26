#include "renderobjects.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderObject::RenderObject() : m_init(false), m_diffuse(1.0f, 1.0f, 1.0f) {
}

/**
 * @brief RenderObject::draw
 * @param id
 */
void RenderObject::draw(GLuint id) {
    if (!m_init) return;

    // See if the shader program has the a_Position attribute
    GLint vertAttribLoc = glGetAttribLocation(id, "a_VertexPosition");
    GLint normAttribLoc = glGetAttribLocation(id, "a_VertexNormal");
    GLint texAttribLoc = glGetAttribLocation(id, "a_TexCoord");
    GLint colAttribLoc = glGetAttribLocation(id, "a_VertexColour");
    GLint matLoc = glGetUniformLocation(id, "u_Material.Ka");


    // Bind the vertex array (there is only one for each call to DrawElements / DrawArrays)
    glBindVertexArray(m_va);

    // Bind the vertex array attribute pointer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(vertAttribLoc);
    glVertexAttribPointer(vertAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Set the normal attributes if they exist
    if (normAttribLoc != -1) {
        glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
        glEnableVertexAttribArray(normAttribLoc);
        glVertexAttribPointer(normAttribLoc, 3, GL_FLOAT, GL_TRUE, 0, 0);
    }

    // Bind the texture array attribute pointer
    if (texAttribLoc != -1) {
        glEnableVertexAttribArray(texAttribLoc);
        glBindBuffer(GL_ARRAY_BUFFER, m_tcbo);
        glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (matLoc != -1) {
        glUniform3fv(glGetUniformLocation(id, "u_Material.Ka"), 1,
                     glm::value_ptr(glm::vec3(0.2,0.2,0.2)));
        glUniform3fv(glGetUniformLocation(id, "u_Material.Kd"), 1,
                     glm::value_ptr(m_diffuse));
        glUniform3fv(glGetUniformLocation(id, "u_Material.Ks"), 1,
                     glm::value_ptr(glm::vec3(1.0,1.0,1.0)));
        glUniform1f(glGetUniformLocation(id, "u_Material.Shininess"), 4.0f);
    }

    if (colAttribLoc != -1) {
        glEnableVertexAttribArray(colAttribLoc);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Enable element array and draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
    glDrawElements(GL_TRIANGLES, m_ntris * 3, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    // Disable the arrays where necessary
    glDisableVertexAttribArray(vertAttribLoc);
    if (texAttribLoc != -1) glDisableVertexAttribArray(texAttribLoc);
}

/**
 * @brief PlaneRenderObject::init
 */
void PlaneRenderObject::init() {
    if (m_init) return;

    m_nverts = 4;
    m_ntris = 2;

    // Create a plane
    glGenVertexArrays(1, &m_va);
    glBindVertexArray(m_va);
    static const GLfloat verts[4*3] = {-1.0f, -0.0, -1.0f,
                                       1.0f, -0.0, -1.0f,
                                       1.0f, -0.0, 1.0f,
                                       -1.0f, -0.0, 1.0f};
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*4*sizeof(GLfloat),
                 verts,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create normals
    static const GLfloat normals[4*3] = {0.0f, 1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f};

    glGenBuffers(1, &m_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*4*sizeof(GLfloat),
                 normals,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    static const GLfloat tex[2*4] = {0.0f, 0.0f,
                                     10.0f, 0.0f,
                                     10.0f, 10.0f,
                                     0.0f, 10.0f};
    glGenBuffers(1, &m_tcbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tcbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2*4*sizeof(GLfloat),
                 tex,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Construct the triangle elements
    static const GLuint tris[6] = {0, 1, 2,
                                   2, 3, 0};
    glGenBuffers(1, &m_eabo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 3*2*sizeof(GLuint),
                 tris,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_init = true;
}

/**
 * @brief SphereRenderObject::init
 */
void SphereRenderObject::init() {
    if (m_init) return;

    m_diffuse = glm::vec3(1.0f, 0.0f, 0.0f);

    int width = 32;
    int height = 16;

    float theta, phi, *dat, *tex;
    int i, j, t, c, ntri, nvec, *idx;

    nvec = (height-2)* width+2;
    ntri = (height-2)*(width-1)*2;
    m_nverts = nvec;
    m_ntris = ntri;

    dat = (float*) malloc( nvec * 3*sizeof(float) );
    idx =   (int*) malloc( ntri * 3*sizeof(int)   );
    tex = (float*) malloc( nvec * 2*sizeof(float));

    for( t=0, c=0, j=1; j<height-1; j++ )
        for(      i=0; i<width; i++ )
        {
            theta = float(j)/(height-1) * M_PI;
            phi   = float(i)/(width-1 ) * M_PI*2;
            dat[t++] =  sinf(theta) * cosf(phi);
            dat[t++] =  cosf(theta);
            dat[t++] = -sinf(theta) * sinf(phi);

            tex[c++] = dat[t-3];
            tex[c++] = dat[t-2];
        }
    dat[t++]=0; dat[t++]= 1; dat[t++]=0;
    tex[c++] = dat[t-3]; tex[c++] = dat[t-2];
    dat[t++]=0; dat[t++]=-1; dat[t++]=0;
    tex[c++] = dat[t-3]; tex[c++] = dat[t-2];
    for( t=0, j=0; j<height-3; j++ )
        for(i=0; i<width-1; i++ )
        {
            idx[t++] = (j  )*width + i  ;
            idx[t++] = (j+1)*width + i+1;
            idx[t++] = (j  )*width + i+1;
            idx[t++] = (j  )*width + i  ;
            idx[t++] = (j+1)*width + i  ;
            idx[t++] = (j+1)*width + i+1;
        }
    for( i=0; i<width-1; i++ )
    {
        idx[t++] = (height-2)*width;
        idx[t++] = i;
        idx[t++] = i+1;
        idx[t++] = (height-2)*width+1;
        idx[t++] = (height-3)*width + i+1;
        idx[t++] = (height-3)*width + i;
    }

    // Create the box with the checkerboard texture
    glGenVertexArrays(1, &m_va);
    glBindVertexArray(m_va);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 nvec*3*sizeof(float),
                 dat,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create normals
    glGenBuffers(1, &m_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    glBufferData(GL_ARRAY_BUFFER,
                 nvec*3*sizeof(float),
                 dat,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //memset(tex, 0, sizeof(float)*2*nvec);
    glGenBuffers(1, &m_tcbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tcbo);
    glBufferData(GL_ARRAY_BUFFER,
                 nvec*2*sizeof(float),
                 tex,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Construct the triangle elements
    glGenBuffers(1, &m_eabo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 ntri*3*sizeof(int),
                 idx,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    delete []dat;
    delete []idx;
    delete []tex;

    m_init = true;
}

/**
 * @brief PlaneRenderObject::init
 */
void BoxRenderObject::init() {
    if (m_init) return;

    m_nverts = 24;
    m_ntris = 12;

    // Create a plane
    glGenVertexArrays(1, &m_va);
    glBindVertexArray(m_va);

    static const GLfloat verts[] = {-1.0f, -1.0f, -1.0f,
                                    -1.0f, 1.0f, -1.0f,
                                    1.0f, 1.0f, -1.0f,
                                    1.0f, -1.0f, -1.0f,

                                    1.0f, -1.0f, -1.0f,
                                    1.0f, 1.0f, -1.0f,
                                    1.0f, 1.0f, 1.0f,
                                    1.0f, -1.0f, 1.0f,

                                    1.0f, -1.0f, 1.0f,
                                    1.0f, 1.0f, 1.0f,
                                    -1.0f, 1.0f, 1.0f,
                                    -1.0f, -1.0f, 1.0f,

                                    -1.0f, -1.0f, 1.0f,
                                    -1.0f, 1.0f, 1.0f,
                                    -1.0f, 1.0f, -1.0f,
                                    -1.0f, -1.0f, -1.0f,

                                    1.0f, -1.0f, 1.0f,
                                    1.0f, -1.0f, -1.0f,
                                    -1.0f, -1.0f, -1.0f,
                                    -1.0f, -1.0f, 1.0f,

                                    -1.0f, 1.0f, 1.0f,
                                    -1.0f, 1.0f, -1.0f,
                                    1.0f, 1.0f, -1.0f,
                                    1.0f, 1.0f, 1.0f};

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*m_nverts*sizeof(GLfloat),
                 verts,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create normals
    static const GLfloat normals[] = {0.0f, 0.0f, -1.0f,
                                         0.0f, 0.0f, -1.0f,
                                         0.0f, 0.0f, -1.0f,
                                         0.0f, 0.0f, -1.0f,
                                         1.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f, 0.0f,
                                         1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f,
                                         0.0f, 0.0f, 1.0f,
                                         0.0f, 0.0f, 1.0f,
                                         0.0f, 0.0f, 1.0f,
                                         -1.0f, 0.0f, 0.0f,
                                         -1.0f, 0.0f, 0.0f,
                                         -1.0f, 0.0f, 0.0f,
                                         -1.0f, 0.0f, 0.0f,
                                         0.0f, -1.0f, 0.0f,
                                         0.0f, -1.0f, 0.0f,
                                         0.0f, -1.0f, 0.0f,
                                         0.0f, -1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f};

    glGenBuffers(1, &m_nbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*m_nverts*sizeof(GLfloat),
                 normals,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    static const GLfloat tex[] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,};
    glGenBuffers(1, &m_tcbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_tcbo);
    glBufferData(GL_ARRAY_BUFFER,
                 2*m_nverts*sizeof(GLfloat),
                 tex,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Construct the triangle elements
    GLuint tris[] = {0, 1, 2,
                     2, 3, 0,
                     4, 5, 6,
                     6, 7, 4,
                     8, 9, 10,
                     10, 11, 8,
                     12, 13, 14,
                     14, 15, 12,
                     16, 17, 18,
                     18, 19, 16,
                     20, 21, 22,
                     22, 23, 20};

    glGenBuffers(1, &m_eabo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 3*m_ntris*sizeof(GLuint),
                 tris,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_init = true;
}

void AxisRenderObject::draw(GLuint id) {
    if (!m_init) return;

    // See if the shader program has the a_Position attribute
    GLint vertAttribLoc = glGetAttribLocation(id, "a_VertexPosition");
    GLint colAttribLoc = glGetAttribLocation(id, "a_VertexColour");

    // Bind the vertex array (there is only one for each call to DrawElements / DrawArrays)
    glBindVertexArray(m_va);

    // Bind the vertex array attribute pointer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(vertAttribLoc);
    glVertexAttribPointer(vertAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (colAttribLoc != -1) {
        glEnableVertexAttribArray(colAttribLoc);
        glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
        glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Enable element array and draw
    glLineWidth(3.0f);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
    glDrawElements(GL_LINES, m_ntris * 2, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glLineWidth(1.0f);
    // Disable the arrays where necessary
    glDisableVertexAttribArray(vertAttribLoc);
}

/**
 * @brief AxisRenderObject::init
 */
void AxisRenderObject::init() {
    if (m_init) return;

    m_nverts = 6;
    m_ntris = 3;

    // Create a plane
    glGenVertexArrays(1, &m_va);
    glBindVertexArray(m_va);
    static const GLfloat verts[6*3] = {0.0f, 0.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, 1.0f};
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*m_nverts*sizeof(GLfloat),
                 verts,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create a colour array
//    static const GLfloat colours[6*3] = {1.0f, 0.0f, 0.0f,
//                                         1.0f, 0.0f, 0.0f,
//                                         0.0f, 1.0f, 0.0f,
//                                         0.0f, 1.0f, 0.0f,
//                                         0.0f, 0.0f, 1.0f,
//                                         0.0f, 0.0f, 1.0f};
    glGenBuffers(1, &m_cbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
    glBufferData(GL_ARRAY_BUFFER,
                 3*m_nverts*sizeof(GLfloat),
                 verts,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Construct the triangle elements (they're actually GL_LINES
    static const GLuint tris[9] = {0, 1,
                                   2, 3,
                                   4, 5};

    glGenBuffers(1, &m_eabo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eabo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 2*m_ntris*sizeof(GLuint),
                 tris,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_init = true;
}
