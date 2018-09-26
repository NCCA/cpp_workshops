#ifndef CONSTRAINTDRAWER_H
#define CONSTRAINTDRAWER_H

#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include "renderobjects.h"

// Need this for getOrientation (might be better in some sort of utility class / function)
#define RADS_TO_DEGREES 57.29578049

class ConstraintDrawer
{
public:
    ConstraintDrawer();
    ~ConstraintDrawer();

    void init();
    void draw(btTypedConstraint *constraint);

private:
    AxisRenderObject *m_axis;
    bool m_init;
};


#endif // CONSTRAINTDRAWER_H
