#include "cubescene.h"

// Needed for memcpy
#include <string.h>

// Use this to keep track of the times
#include <sys/time.h>

#include <iostream>

/**
 * @brief CubeScene::CubeScene
 */
CubeScene::CubeScene() : Scene(), m_rotation(0.0f) {
}

/**
 * @brief CubeScene::~CubeScene
 */
CubeScene::~CubeScene() {
    // Only delete allocated memory if we've initialised the class
    if (m_isInit) {
        delete [] m_verts;
        delete [] m_tris;
    }
}

/**
 * @brief CubeScene::init
 */
void CubeScene::init() {
    // Sanity check - if we've already initialised this class we shouldn't be here
    if (m_isInit) return;

    // Create the cube geometry (tedious)
    m_nverts = 8;
    m_verts = new Vertex[m_nverts];
    GLfloat vertData[] = {-1.0f, -1.0f, -1.0f,
                          -1.0f, 1.0f, -1.0f,
                          1.0f, 1.0f, -1.0f,
                          1.0f, -1.0f, -1.0f,
                          -1.0f, -1.0f, 1.0f,
                          -1.0f, 1.0f, 1.0f,
                          1.0f, 1.0f, 1.0f,
                          1.0f, -1.0f, 1.0f};
    // Copy the chunk of data to the local member
    memcpy(m_verts, vertData, sizeof(Vertex) * m_nverts);

    // Create the indexed triangle array (more tedious)
    m_ntris = 12;
    m_tris = new Triangle[m_ntris];
    GLuint triData[] = {0, 1, 2,
                        2, 3, 0,
                        1, 5, 6,
                        6, 2, 1,
                        6, 7, 5,
                        7, 4, 5,
                        7, 3, 2,
                        3, 0, 2,
                        5, 4, 1,
                        1, 0, 4,
                        6, 7, 2,
                        2, 7, 3};


    // Copy the chunk of data to the local member
    memcpy(m_tris, triData, sizeof(Triangle) * m_ntris);

    // Call the parent class initialisation!
    Scene::init();

    // Disable lighting for this example
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

/**
 * @brief CubeScene::draw
 */
void CubeScene::draw() {
    if (!m_isInit) return;

    // Clear the display buffer (colour set by glClearColor)
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Set our scene transformations
    glMatrixMode(GL_MODELVIEW); // set the modelview matrix here
    glLoadIdentity(); // Set it to the identity (no trasnformations
    glTranslatef(0.0f, 0.0f, -4.0f); // Translate it into the view
    glRotatef(m_rotation, 0.0f, 1.0f, 0.0f); // Rotate about the y axis (try swapping this with the line above!)

    GLuint i,j;
    glEnable(GL_POLYGON_OFFSET_FILL); // use a polygon in z-buffer to make sure we see the lines
    glPolygonOffset(1.0f, 1.0f); // Apply the following z-buffer offset (clever trick for wireframe)
    // Draw the triangles of our cube
    glColor3f(1.0f,1.0f,1.0f); // Set colour to white
    glBegin(GL_TRIANGLES); // Triangles take 3 verts
    for (i = 0; i < m_ntris; ++i) {
        for (j = 0; j < 3; ++j) {

            glVertex3f(m_verts[m_tris[i].idx[j]].x, m_verts[m_tris[i].idx[j]].y, m_verts[m_tris[i].idx[j]].z);
        }
    }
    glEnd(); // GL_TRIANGLES
    glDisable(GL_POLYGON_OFFSET_FILL);

    // Draw the line edges of our cube
    glLineWidth(2.0f); // make the edges thicker
    glColor3f(0.0f,0.0f,0.0f); // draw in black
    for (i = 0; i < m_ntris; ++i) {
        glBegin(GL_LINE_LOOP); // Line loops close the loop by connecting the last to first vertex at glEnd()
        for (j = 0; j < 3; ++j) {
            glVertex3f(m_verts[m_tris[i].idx[j]].x, m_verts[m_tris[i].idx[j]].y, m_verts[m_tris[i].idx[j]].z);
        }
        glEnd(); //GL_LINE_LOOP
    }

}

/**
 * @brief CubeScene::update
 */
void CubeScene::update() {
    // Call the parent class update function to make sure m_elapsedTime has been updated
    Scene::update();

    // Use the elapsed time to do some sort of animation
    m_rotation = GLfloat(m_elapsedTime) * 100.0f;
}
