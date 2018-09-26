#ifndef BODYDRAWER_H
#define BODYDRAWER_H

#include "glinc.h"
#include "renderobjects.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

// Need this for getOrientation (might be better in some sort of utility class / function)
#define RADS_TO_DEGREES 57.29578049

/**
 * @brief The BodyDrawer class
 */
class BodyDrawer
{
public:
    BodyDrawer();
    ~BodyDrawer();

    void init();
    void draw(btRigidBody *body);
private:
    bool m_init;
    PlaneRenderObject *m_plane;
    SphereRenderObject *m_sphere;
    BoxRenderObject *m_box;
    GLuint m_texID;

    void drawCollisionShape(btCollisionShape */*shape*/, glm::mat4 /*mm*/);

};



#endif // BODYDRAWER_H
