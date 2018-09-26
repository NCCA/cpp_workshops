#include "passivewalkerscene.h"
#include <stdio.h>

PassiveWalkerScene::PassiveWalkerScene() : Scene()
{
    // Create our initial conditions
    InitialConditions c;

    // Create a ground plane
    btStaticPlaneShape *groundShape = new btStaticPlaneShape(btVector3(0.0,cos(c.slope),-sin(c.slope)),0);
    btMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-0.2,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    groundRigidBodyCI.m_restitution = 0.8;
    btRigidBody *ground = new btRigidBody(groundRigidBodyCI);
    m_dynamicsWorld->addRigidBody(ground);
    m_bodies.push_back(ground);

    // Set up some common parameters
    btTransform transform, com_transform;
    btVector3 localInertia(0,0,0); btScalar mass(c.m_h);
    btMotionState *state;
    btCollisionShape *shape;
    //btCompoundShape *compound;
    btRigidBody *pelvis, *lshank, *lthigh, *rshank, *rthigh;
    double r = 0.05 * c.L; // radius of capsule caps
    double w = 0.1 * c.L; // width of pelvis (space between legs)

    // Create the pelvis
    transform.setIdentity();
    transform.getBasis().setEulerZYX(0.0, 0.0, M_PI*0.5);
    transform.setOrigin(btVector3(0.0, c.L + r, 0.0));
    shape = new btCapsuleShape(r, w + 2.0*r);
    shape->calculateLocalInertia(mass,localInertia);
    state = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo pelvisInfo(mass,state,shape,localInertia);
    pelvisInfo.m_restitution = 1.0;
    pelvis = new btRigidBody(pelvisInfo);
    m_bodies.push_back(pelvis); m_dynamicsWorld->addRigidBody(pelvis);

    // Create the left shank
    transform.setIdentity();
    transform.setOrigin(btVector3(w, 0.5*c.l_s, 0.0));
    shape = new btCapsuleShape(r, c.l_s - 2.0*r);
    mass = c.m_s;
    shape->calculateLocalInertia(mass,localInertia);
    state = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo lshankInfo(mass,state,shape,localInertia);
    lshankInfo.m_restitution = 1.0;
    lshank = new btRigidBody(lshankInfo);
    m_bodies.push_back(lshank); m_dynamicsWorld->addRigidBody(lshank);

    // Create the left thigh
    transform.setIdentity();
    transform.setOrigin(btVector3(w, c.l_s + 0.5 * c.l_t, 0.0));
    shape = new btCapsuleShape(r, c.l_t - 2.0*r);
    mass = c.m_t;
    shape->calculateLocalInertia(mass,localInertia);
    state = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo lthighInfo(mass,state,shape,localInertia);
    lthighInfo.m_restitution = 1.0;
    lthigh = new btRigidBody(lthighInfo);
    m_bodies.push_back(lthigh); m_dynamicsWorld->addRigidBody(lthigh);

//    // Create the right shank
    transform.setIdentity();
    transform.setOrigin(btVector3(-w, 0.5*c.l_s, 0.0));
    shape = new btCapsuleShape(r, c.l_s - 2.0*r);
    mass = c.m_s;
    shape->calculateLocalInertia(mass,localInertia);
    state = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rshankInfo(mass,state,shape,localInertia);
    rshankInfo.m_restitution = 1.0;
    rshank = new btRigidBody(rshankInfo);    
    m_bodies.push_back(rshank); m_dynamicsWorld->addRigidBody(rshank);

    // Create the left thigh
    transform.setIdentity();
    transform.setOrigin(btVector3(-w, c.l_s + 0.5 * c.l_t, 0.0));
    shape = new btCapsuleShape(r, c.l_t - 2.0*r); mass = c.m_t;
    shape->calculateLocalInertia(mass,localInertia);
    state = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rthighInfo(mass,state,shape,localInertia);
    rthighInfo.m_restitution = 1.0;
    rthigh = new btRigidBody(rthighInfo);
    m_bodies.push_back(rthigh); m_dynamicsWorld->addRigidBody(rthigh);

    // Now create the constraints
    btTransform localA, localB;
    localA.setIdentity(); localB.setIdentity();
    localA.getBasis().setEulerZYX(0,-M_PI*0.5,0);
    localA.setOrigin(btVector3(0.0, -0.5*c.l_t,0.0));
    localB.getBasis().setEulerZYX(0.0,-M_PI*0.5,0);
    localB.setOrigin(btVector3(0.0, 0.5*c.l_s,0.0));
    btHingeConstraint *lknee =  new btHingeConstraint(*lthigh, *lshank, localA, localB);
    lknee->setLimit(0.0, btScalar(M_PI));
    lknee->setMotorTarget(M_PI, 10.0);
    m_constraints.push_back(lknee);
    m_dynamicsWorld->addConstraint(lknee);

//    // Same parameters for the right knee
    btHingeConstraint *rknee =  new btHingeConstraint(*rthigh, *rshank, localA, localB);
    rknee->setLimit(0.0, btScalar(M_PI));
    m_constraints.push_back(rknee);
    m_dynamicsWorld->addConstraint(rknee);

//    // Now connect up the hips to the thighs with hips, first the right hip . . .
    localA.setIdentity(); localB.setIdentity();
    localA.getBasis().setEulerZYX(M_PI*0.5,M_PI*0.5,0.0);
    localA.setOrigin(btVector3(-r, w, 0.0));
    localB.getBasis().setEulerZYX(0.0,M_PI*0.5,0);
    localB.setOrigin(btVector3(0.0, 0.5*c.l_t, 0.0));
    btHingeConstraint *rhip =  new btHingeConstraint(*pelvis, *rthigh, localA, localB);
    rhip->setLimit(btScalar(-M_PI), btScalar(M_PI));
    m_constraints.push_back(rhip);
    m_dynamicsWorld->addConstraint(rhip);

//    // . . . and now the left hip
    localA.setOrigin(btVector3(-r, -w, 0.0));
    btHingeConstraint *lhip =  new btHingeConstraint(*pelvis, *lthigh, localA, localB);
    lhip->setLimit(btScalar(-M_PI), btScalar(M_PI));

    // Set the initial rotation
    localA.setIdentity();
    localA.getBasis().setEulerZYX(0.0, c.q[1], 0.0);
    lhip->setFrames(localA * lhip->getAFrame(), lhip->getBFrame());

    localA.setIdentity();
    localA.getBasis().setEulerZYX(0.0, c.q[2], 0.0);
    rhip->setFrames(localA * rhip->getAFrame(), rhip->getBFrame());

    m_constraints.push_back(lhip);
    m_dynamicsWorld->addConstraint(lhip);

    // Create a box for testing

//    transform.setIdentity();
//    transform.setOrigin(btVector3(0.0, 10.0f, 0.0));
//    shape = new btBoxShape(btVector3(2.0f, 0.1f, 2.0f));
//    shape->calculateLocalInertia(mass,localInertia);
//    state = new btDefaultMotionState(transform);
//    btRigidBody::btRigidBodyConstructionInfo boxInfo(mass,state,shape,localInertia);
//    btRigidBody *box = new btRigidBody(boxInfo);
//    m_bodies.push_back(box); m_dynamicsWorld->addRigidBody(box);


}


/**
 * @brief PassiveWalkerScene::applyHingeTorque
 * Store this here for the time being: it will probably need to live somewhere more useful.
 * From http://bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=4852&view=next
 * @param pHinge
 * @param T
 */
void PassiveWalkerScene::applyHingeTorque(btHingeConstraint *pHinge, btScalar T) {
    btVector3 hingeAxisLocal = pHinge->getAFrame().getBasis().getColumn(2); // z-axis of constraint frame
    btVector3 hingeAxisWorld = pHinge->getRigidBodyA().getWorldTransform().getBasis() * hingeAxisLocal;
    btVector3 hingeTorque = T * hingeAxisWorld;
    pHinge->getRigidBodyA().applyTorque(hingeTorque);
    pHinge->getRigidBodyB().applyTorque(-hingeTorque);
    fprintf(stderr, "PassiveWalkerScene::applyHingeTorque(%f): [%f,%f,%f]\n",T,hingeTorque.getX(),hingeTorque.getY(),hingeTorque.getZ());
}
