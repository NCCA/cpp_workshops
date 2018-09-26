#ifndef SCENE_H
#define SCENE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

/**
 * @brief The Scene class
 */
class Scene
{
public:
    /// A struct to hold vertices
    typedef struct {GLfloat x,y,z;} Vertex;

    /// A struct to hold triangles
    typedef struct {GLuint idx[3];} Triangle;

public:
    /// A constructor, called when this class is instanced in the form of an object
    Scene();

    /// A virtual destructor, in case we want to inherit from this class
    virtual ~Scene();

    /// Initialises the scene, called before render().
    virtual void init();

    /// Called when the window size changes (for example, if the user maximises the window).
    virtual void resize(int w, int h);

    /// Called frequently, used if you are using some form of animation
    virtual void update();

    /// Called most frequently, redraws the scene.
    virtual void draw();

protected: // Protected means that it is accessible to derived classes
    /// Keep track of whether this has been initialised - otherwise it won't be ready to draw!
    bool m_isInit;

    /// The time since the object was initialised, which is used for animation purposes
    double m_startTime;

    /// A member that is updated when update() is called indicating the elapsed time
    double m_elapsedTime;

};

#endif // SCENE_H
