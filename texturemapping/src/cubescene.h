#ifndef CUBESCENE_H
#define CUBESCENE_H

#include "scene.h"

/**
 * @brief The CubeScene class
 */
class CubeScene : public Scene
{
public:
    /// Constructor
    CubeScene();

    /// Destructor
    virtual ~CubeScene();

    /// Initialises the scene, called before render().
    virtual void init();

    /// Called when the window size changes (for example, if the user maximises the window).
    // virtual void resize(int w, int h);

    /// Called frequently, used if you are using some form of animation
    virtual void update();

    /// Called most frequently, redraws the scene.
    virtual void draw();

private:
    /// Dynamic storage for our point data
    Vertex *m_verts;
    GLuint m_nverts;

    /// Dynamic storage for our triangle data
    Triangle *m_tris;
    GLuint m_ntris;

    /// The rotation of this object, used for animation
    GLfloat m_rotation;
};

#endif // CUBESCENE_H
