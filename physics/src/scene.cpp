#include "scene.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sys/time.h>

/**
 * @brief Scene::Scene
 */
Scene::Scene() : m_lastx(0), m_lasty(0), m_lastTime(0.0), m_mouseZoom(-25.0f), m_mouseRotateX(45.0f), m_mouseRotateY(0.0f), m_mouseButtons(0)
{
    // Create the bullet world
    m_broadphase = new btDbvtBroadphase();
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0,-10,0));
}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene() {
    BodyVector::iterator bit;
    for (bit = m_bodies.begin(); bit != m_bodies.end(); ++bit) {
        delete (*bit)->getMotionState();
        delete (*bit)->getCollisionShape();
        delete (*bit);
    }
    ConstraintVector::iterator cit;
    for (cit = m_constraints.begin(); cit != m_constraints.end(); ++cit) delete (*cit);

    delete m_dynamicsWorld;
    delete m_solver;
    delete m_dispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

/**
 * @brief Scene::update
 */
void Scene::update() {
    // Some stuff we need to perform timings
    struct timeval tim;

    // Retrieve the current time in nanoseconds (accurate to 10ns)
    gettimeofday(&tim, NULL);
    double now =tim.tv_sec+(tim.tv_usec * 1e-6);

    // Increment the rotation based on the time elapsed since we started running
    double m_elapsedTime = now - m_lastTime;

    // Update the scene!
    //std::cerr << "stepSimulation("<<m_elapsedTime<<")\n";
    m_dynamicsWorld->stepSimulation(m_elapsedTime,10);

    // Refresh our last time
    m_lastTime = now;
}

/**
 * @brief Scene::init
 */
void Scene::init() {
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable( GL_POINT_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable( GL_MULTISAMPLE_ARB);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Initialise the shader we'll use to draw the scene
    m_phongShader.init("shaders/phong.vert", "shaders/phong.frag");

    // This shader is used for visualisation
    m_basicShader.init("shaders/basic.vert", "shaders/basic.frag");

    // Initialise the gl drawer object
    m_bodyDrawer.init();
    m_constraintDrawer.init();

    // Set our start time by using the gettimeofday function (accurate to 10 nanosecs)
    struct timeval tim;
    gettimeofday(&tim, NULL);
    m_lastTime = tim.tv_sec+(tim.tv_usec * 1e-6);
}

/**
 * @brief Scene::draw
 */
void Scene::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader.bind();
    // Set up the camera parameters
    GLint vLoc = glGetUniformLocation(m_phongShader.id(), "u_ViewMatrix");

    // Calculate the view matrix from the trackball
    glm::mat4 mv(1.0);
    mv = glm::translate(mv, glm::vec3(0.0f, 0.0f, m_mouseZoom));
    mv = glm::rotate(mv, glm::radians(m_mouseRotateX), glm::vec3(1.0f, 0.0f, 0.0f));
    mv = glm::rotate(mv, glm::radians(m_mouseRotateY), glm::vec3(0.0f, 1.0f, 0.0f));

    if (vLoc != -1) {
        glUniformMatrix4fv(/*location*/vLoc,
                           /*# of matrices*/1,
                           /*transpose?*/GL_FALSE,
                           /*The matrix pointer*/ glm::value_ptr(mv));
    }

    // Set the shader light parameters (fixed)
    glUniform4fv(glGetUniformLocation(m_phongShader.id(), "u_Light.Position"), 1,
                 glm::value_ptr(glm::vec4(0.0f, 20.0f, 0.0f,1.0)));
    glUniform3fv(glGetUniformLocation(m_phongShader.id(), "u_Light.La"), 1,
                 glm::value_ptr(glm::vec3(0.5,0.5,0.5)));
    glUniform3fv(glGetUniformLocation(m_phongShader.id(), "u_Light.Ld"), 1,
                 glm::value_ptr(glm::vec3(1.0,1.0,1.0)));
    glUniform3fv(glGetUniformLocation(m_phongShader.id(), "u_Light.Ls"), 1,
                 glm::value_ptr(glm::vec3(0.1,0.1,0.1)));


    // Call the draw function on all bodies in the scene
    BodyVector::iterator bit;
    for (bit = m_bodies.begin(); bit != m_bodies.end(); ++bit)
        m_bodyDrawer.draw(*bit);

    m_phongShader.unbind();

    m_basicShader.bind();
    // Set up the camera parameters
    vLoc = glGetUniformLocation(m_basicShader.id(), "u_ViewMatrix");
    if (vLoc != -1) {
        glUniformMatrix4fv(/*location*/vLoc,
                           /*# of matrices*/1,
                           /*transpose?*/GL_FALSE,
                           /*The matrix pointer*/ glm::value_ptr(mv));
    }
    ConstraintVector::iterator cit;
    for (cit = m_constraints.begin(); cit != m_constraints.end(); ++cit)
        m_constraintDrawer.draw(*cit);

    m_basicShader.unbind();
}

/**
 * @brief Scene::resize
 * @param w
 * @param h
 */
void Scene::resize(GLsizei w, GLsizei h) {
    // Use this function to ensure that the aspect ratio is maintained
    GLfloat r = GLfloat(w) / GLfloat(h);
    glm::mat4 proj = glm::perspective(45.0f, r, 0.5f, 100.0f);

    // Specify the extents of our GL window
    glViewport(0,0, w, h);    

    // Set up the default camera parameters
    GLint proj_id;

    m_phongShader.bind();
    proj_id = glGetUniformLocation(m_phongShader.id(), "u_ProjectionMatrix");
    if (proj_id != -1) {
        glUniformMatrix4fv(/*location*/ proj_id,
                       /*# of matrices*/ 1,
                       /*transpose?*/ GL_FALSE,
                       /*The matrix pointer*/ glm::value_ptr(proj));
    } else {
        std::cerr << "u_ProjectionMatrix not found in shader "<<m_phongShader.id()<<"\n";
    }
    m_phongShader.unbind();

    m_basicShader.bind();
    proj_id = glGetUniformLocation(m_basicShader.id(), "u_ProjectionMatrix");
    if (proj_id != -1) {
        glUniformMatrix4fv(/*location*/ proj_id,
                       /*# of matrices*/ 1,
                       /*transpose?*/ GL_FALSE,
                       /*The matrix pointer*/ glm::value_ptr(proj));
    } else {
        std::cerr << "u_ProjectionMatrix not found in shader "<<m_basicShader.id()<<"\n";
    }
    m_basicShader.unbind();
}

/**
 * @brief Scene::mouseMove
 * @param x
 * @param y
 */
void Scene::mouseMove(const int &x, const int &y) {
    float dx = (float)(x - m_lastx);
    float dy = (float)(y - m_lasty);

    if (m_mouseButtons & MOUSEBUTTON_LEFT) {
        m_mouseRotateX += dy * 0.1f;
        m_mouseRotateY += dx * 0.1f;
    } else if (m_mouseButtons & MOUSEBUTTON_RIGHT) {
        m_mouseZoom += dy * 0.1f;
    }
    m_lastx = x;
    m_lasty = y;
}

/**
 * @brief Scene::mouseClick
 * @param button
 * @param isDown
 * @param x
 * @param y
 */
void Scene::mouseClick(const MouseButton &button, const bool &isDown, const int &x, const int &y) {
    if (isDown) {
        m_mouseButtons |= 1<<button;
    } else {
        m_mouseButtons = 0;
    }
    m_lastx = x;
    m_lasty = y;
}
