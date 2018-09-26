#include "testscene.h"

/**
 * @brief TestScene::TestScene
 * Create a ball and a plane to test the renderer
 */
TestScene::TestScene() : Scene()
{
    // Create a ground plane
    btStaticPlaneShape *groundShape = new btStaticPlaneShape(btVector3(0,1,0.1),0);
    btMotionState *groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    groundRigidBodyCI.m_restitution = 1.0f;
    btRigidBody *ground = new btRigidBody(groundRigidBodyCI);
    m_dynamicsWorld->addRigidBody(ground);
    m_bodies.push_back(ground);

    // Create a sphere
    btSphereShape *sphereShape = new btSphereShape(1);
    btMotionState *motionState = new btDefaultMotionState(btTransform(btQuaternion(0.0,0.0,0.0,1),btVector3(0.0,5.0,0.0)));
    btScalar mass = 10.0f;
    btVector3 fallInertia(0,0,0);
    sphereShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,motionState,sphereShape,fallInertia);
    fallRigidBodyCI.m_restitution = 1.0f;
    btRigidBody *sphere = new btRigidBody(fallRigidBodyCI);
    sphere->setAngularVelocity(btVector3(0.0, 0.0, 0.0));
    m_dynamicsWorld->addRigidBody(sphere);
    m_bodies.push_back(sphere);
}


TestScene::~TestScene() {
}
