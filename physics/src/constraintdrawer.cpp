#include "constraintdrawer.h"
#include <iostream>
/**
 * @brief ConstraintDrawer::ConstraintDrawer
 */
ConstraintDrawer::ConstraintDrawer() : m_init(false)
{
    m_axis = new AxisRenderObject();
}

/**
 * @brief ConstraintDrawer::~ConstraintDrawer
 */
ConstraintDrawer::~ConstraintDrawer() {
    delete m_axis;
}

/**
 * @brief ConstraintDrawer::init
 */
void ConstraintDrawer::init() {
    if (m_init) return;
    m_axis->init();
    m_init = true;
}

/**
 * @brief ConstraintDrawer::draw
 * @param constraint
 */
void ConstraintDrawer::draw(btTypedConstraint *constraint) {
    if (!m_init) return;

    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &id);

    // Retrieve shader properties
    GLint mmLoc = glGetUniformLocation(id, "u_ModelMatrix");

    // Determine the type of the collision so we can draw a tailored output
    int constraintType = constraint->getConstraintType();

    btTransform trans;
    btRigidBody *body;
    GLfloat mat[16];
    btHingeConstraint *hingeConstraint;

    switch (constraintType) {
    case POINT2POINT_CONSTRAINT_TYPE:
        break;
    case HINGE_CONSTRAINT_TYPE:
        hingeConstraint = dynamic_cast<btHingeConstraint*>(constraint);
        if (hingeConstraint != NULL) {
            body = &hingeConstraint->getRigidBodyA();
            trans = body->getCenterOfMassTransform() * hingeConstraint->getAFrame();
            trans.getOpenGLMatrix(mat);
            if (mmLoc != -1) glUniformMatrix4fv(mmLoc, 1, GL_FALSE, mat);
            m_axis->draw(id);
            body = &hingeConstraint->getRigidBodyB();
            trans = body->getCenterOfMassTransform() * hingeConstraint->getBFrame();
            trans.getOpenGLMatrix(mat);
            if (mmLoc != -1) glUniformMatrix4fv(mmLoc, 1, GL_FALSE, mat);
            m_axis->draw(id);
        }
        break;
    case CONETWIST_CONSTRAINT_TYPE:
        break;
    case D6_CONSTRAINT_TYPE:
        break;
    case SLIDER_CONSTRAINT_TYPE:
        break;
    case CONTACT_CONSTRAINT_TYPE:
        break;
    case D6_SPRING_CONSTRAINT_TYPE:
        break;
    case GEAR_CONSTRAINT_TYPE:
        break;
    case MAX_CONSTRAINT_TYPE:
        break;
    default:
        break;
    }


}
