#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "bodydrawer.h"
#include "constraintdrawer.h"
#include "shader.h"
#include "glinc.h"

typedef std::vector<btRigidBody*> BodyVector;
typedef std::vector<btTypedConstraint*> ConstraintVector;

/**
 * @brief The Scene class
 */
class Scene
{
public:
    typedef enum {
        MOUSEBUTTON_UNKNOWN = 0,
        MOUSEBUTTON_LEFT = 1,
        MOUSEBUTTON_MIDDLE = 2,
        MOUSEBUTTON_RIGHT = 3
    } MouseButton;
public:
    Scene();
    virtual ~Scene();
    virtual void draw();
    virtual void init();
    virtual void resize(GLsizei w, GLsizei h);
    virtual void update();
    virtual void mouseMove(const int &x, const int &y);
    virtual void mouseClick(const MouseButton &button, const bool &isDown, const int &x, const int &y);
    virtual void setLastMouseState(const int &x, const int &y) {m_lastx = x; m_lasty = y;}

protected:    
    /// A drawer object
    BodyDrawer m_bodyDrawer;
    ConstraintDrawer m_constraintDrawer;

    /// Our GLSL shader
    Shader m_phongShader, m_basicShader;

    /// Bullet stuff
    btBroadphaseInterface* m_broadphase;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;

    BodyVector m_bodies; //< A vector holding bullet bodies
    ConstraintVector m_constraints; //< A vector holding constraints

    int m_lastx, m_lasty; //< Used for mouse position tracking
    double m_lastTime;     //< For the update function
    float m_mouseZoom;    //< Current mouse zoom
    float m_mouseRotateX, m_mouseRotateY; //< Current mouse rotation
    int m_mouseButtons; //< Keeps track of all buttons currently clicked
};

#endif // SCENE_H
