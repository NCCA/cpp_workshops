#include "terrainscene.h"
#include <iostream>

/**
 * @brief TerrainScene::TerrainScene
 * Construct the terrain scene by allocating some memory for our vertex data and triangle indices
 */
TerrainScene::TerrainScene(int xsize, int ysize)
    : Scene(), m_nVerts(xsize*ysize), m_nTris((xsize-1)*(ysize-1)*2), m_xsize(xsize), m_ysize(ysize) {
    // Note that each vertex is made up of 9 attributes:
    // (x,y,z),(nx,ny,nz),(r,g,b)
    m_data = (GLfloat*) malloc(sizeof(GLfloat) * // Size of the data type in bytes
                               m_nVerts * // The number of vertices
                               9); // Consists of vertex position, normal and colour information

    // Create the triangle indices for our scene
    m_tris = (GLuint*) malloc(sizeof(GLuint) * // The size of the data type in bytes
                             (xsize-1) * (ysize-1) * // There is one less row / col in the triangle strip
                             3 * // There are 3 indices for each triangle
                             2); // There are 2 triangles in each 4 vertex patch
}

/**
 * @brief TerrainScene::~TerrainScene
 */
TerrainScene::~TerrainScene() {
    free(m_data);
    free(m_tris);
}

/**
 * @brief TerrainScene::init Initialise the data and opengl stuff needed to display the scene.
 * This is where the normals are created.
 */
void TerrainScene::init() {
    Scene::init();

    // Enable material properties for our mesh
    GLfloat matAmb[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat matDiff[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matSpec[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat matShine = 1000.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShine);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // Comment out these two lines when you are ready to test your normals
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);

    unsigned int i, j;

    // Build the triangle data
    GLuint *iptr = m_tris;
    for (i = 1; i < m_xsize; ++i) {
        for (j = 1; j < m_ysize; ++j) {
            // Triangles in the patch are defined according to this indexing scheme:
            //   (i-1)+(j-1)*xsz-------i+(j-1)*xsz
            //        |    \             |
            //        |      \           |
            //        |        \         |
            //        |          \       |
            //        |            \     |
            //        |              \   |
            //    (i-1)+j*xsz----------i+j*xsz

            // The first triangle in this patch
            iptr[0] = (i-1)+(j-1)*m_xsize; iptr[1] = i+j*m_xsize; iptr[2] = i+(j-1)*m_xsize;

            // The second triangle in this patch
            iptr[3] = iptr[0]; iptr[4] = (i-1)+j*m_xsize; iptr[5] = iptr[1];

            // Increment our pointer position
            iptr += 6;
        }
    }

    // Build the vertex data
    GLfloat xpart = 2.0f / GLfloat(m_xsize-1);
    GLfloat ypart = 2.0f / GLfloat(m_ysize-1);
    GLfloat *ptr = m_data;
    GLfloat x,y,z,r,g,b;

    for (i = 0; i < m_xsize; ++i) {
        for (j = 0; j < m_ysize; ++j) {
            // Calculate the position of the vertex
            x = i*xpart-1.0; y=j*ypart-1.0;
            z = GLfloat(m_noise.GetValue(x, y, 0.0));
            ptr[0] = x;
            ptr[1] = y;
            ptr[2] = 0.2f * z;

            // Initialise our normals to zero
            ptr[3] = 0.0f;
            ptr[4] = 0.0f;
            ptr[5] = 0.0f;

            // Compute the colour of each vertex
            b = (z+1.0f) * 0.5f;
            g = 1.0f - b;
            r = 0.0f;
            ptr[6] = r;
            ptr[7] = g;
            ptr[8] = b;
            ptr += 9; // Jump to the next load of data
        }
    }

    // Here you'll need to calculate the vertex normals. Recall (in my slides)
    // that you compute the face normals, and accumulate the (unnormalised) face normals
    // incident on each vertex together, only normalising the result afterwards. There
    // are a few clever tricks that you can use to speed this up - I'll leave it up
    // to you to figure these out.

}

/**
 * @brief TerrainScene::resize - this didn't really need to be overloaded - I was just experimenting with other cameras.
 * @param w Width
 * @param h Height
 */
void TerrainScene::resize(int w, int h) {
    if (!m_isInit) return;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -5.0, 5.0);
    gluPerspective(60.0, float(w) / float(h), 0.1, 10.0);

    glViewport(0,0,w,h);

    glMatrixMode(GL_MODELVIEW);
}

/**
 * @brief TerrainScene::draw Display the data in triangles and/or points
 */
void TerrainScene::draw() {
    // Draw the parent class draw routine to clear the image
    Scene::draw();

    // Position the object in front of the camera and rotate it
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -2.0f);
    glRotatef(m_rotation, 0.0f, 1.0f, 0.0f);

    unsigned int i,j, triIdx;

    // Draw the data as points using pointer arithmetic!
//    GLfloat *ptr = m_data; // For scary pointer arithmetic
//    glPointSize(1.0f); // Scale the points if needed
//    glBegin(GL_POINTS);
//    for (i=0; i<m_nVerts; ++i) {
//        glNormal3fv(ptr+3);
//        glColor3fv(ptr+6);
//        glVertex3fv(ptr);
//        ptr += 9;
//    }
//    glEnd(); //GL_POINTS

    // Draw triangles based on the triangle index
    glBegin(GL_TRIANGLES);
    for (i=0; i<m_nTris; ++i) {
        for (j=0; j<3; ++j) {
            triIdx = m_tris[i*3+j];
            glNormal3fv(&(m_data[triIdx*9]) + 3);
            glColor3fv(&(m_data[triIdx*9]) + 6);
            glVertex3fv(&(m_data[triIdx*9]));
        }
    }
    glEnd(); // GL_TRIANGLES

    glPopMatrix();
}

/**
 * @brief TerrainScene::update rotate the scene based on the elapsed time.
 */
void TerrainScene::update() {
    Scene::update();

    // Use the elapsed time to do some sort of animation
    m_rotation = GLfloat(m_elapsedTime) * 50.0f;
}
