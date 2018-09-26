#include "bodydrawer.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL.h"
#include <glm/ext.hpp>

BodyDrawer::BodyDrawer() : m_init(false) {
    m_plane = new PlaneRenderObject();
    m_sphere = new SphereRenderObject();
    m_box = new BoxRenderObject();
}

BodyDrawer::~BodyDrawer() {
    if (m_init) {
        delete m_plane;
        delete m_sphere;
        delete m_box;
    }
}

/**
 * @brief BodyDrawer::init Initialise all the types of rigid bodies that we'd want to draw
 */
void BodyDrawer::init() {
    if (m_init) return;

    // Create a plane
    m_plane->init();

    // Create a sphere
    m_sphere->init();

    // Create a box
    m_box->init();

    // Create a capsule

    // Create a default texture
    // Load the image from a file
    m_texID = SOIL_load_OGL_texture(
                "data/textures/checkerboard.jpg",    /*filename*/
                SOIL_LOAD_AUTO,         /*channels (automatic)*/
                SOIL_CREATE_NEW_ID,     /*reuse id or request new one*/
                SOIL_FLAG_MIPMAPS |     /*other flags*/
                SOIL_FLAG_INVERT_Y |
                SOIL_FLAG_NTSC_SAFE_RGB);
    if( m_texID == 0 ) {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }

    // Activate and bind our new texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    // Set up parameters for our texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

/**
 * @brief BodyDrawer::draw Draw the specified rigid body
 */
void BodyDrawer::draw(btRigidBody *body) {
    // The Model Matrix is going to be different for each body type, so this gets a bit messy
    GLfloat mat[16];
    btTransform trans = body->getCenterOfMassTransform();
    trans.getOpenGLMatrix(mat);
    glm::mat4 mm(1.0f);
    memcpy(&(mm[0]), mat, sizeof(GLfloat)*16);

    // A body may have a compound collision shape. This means that we must spawn an extra draw job for each shape
    btCollisionShape *shape = body->getCollisionShape();

    int i;
    if (shape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE) {
        // The shape consists of many subshapes. Each must be drawn.
        for (i=0; i<((btCompoundShape*) shape)->getNumChildShapes(); ++i) {
            drawCollisionShape(((btCompoundShape*) shape)->getChildShape(i), mm);
        }
    } else {
        // It's just a regular shape, so draw it as usual
        drawCollisionShape(shape, mm);
    }

}


void BodyDrawer::drawCollisionShape(btCollisionShape *shape, glm::mat4 mm) {
    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &id);

    // Retrieve shader properties
    GLint texLoc = glGetUniformLocation(id, "u_Texture");
    GLint mmLoc = glGetUniformLocation(id, "u_ModelMatrix");
    GLint nmLoc = glGetUniformLocation(id, "u_NormalMatrix");

    // Activate our texture 0
    if (texLoc != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texID);
        glUniform1i(texLoc, 0); // The value is zero - the active texture
    }

    double r,h;
    btSphereShape *sphereShape;
    btCapsuleShape *capsuleShape;
    btStaticPlaneShape *planeShape;
    btBoxShape *boxShape;

    glm::vec3 axis, N, N_dash;
    btVector3 extents;
    float angle, inv_mag, mag;

    switch (shape->getShapeType()) {
    case STATIC_PLANE_PROXYTYPE:
        //std::cerr << "STATIC_PLANE_PROXYTYPE\n";
        // Set the matrices
        // Need to retrieve the surface normal
        // Given the old normal N and the new normal N' you can obtain the rotation by:
        // RotationAxis = cross(N, N')
        // RotationAngle = arccos(dot(N, N') / (|N| * |N'|))

        planeShape = dynamic_cast<btStaticPlaneShape*>(shape);
        if (planeShape == NULL) {
            std::cerr << "BodyDrawer::draw(): Critical Error: failure to dynamic cast a plane shape although it's shapeType says it's ok!\n";
            exit(0);
        }
        N = glm::vec3(0,1,0);
        N_dash = glm::vec3(planeShape->getPlaneNormal().getX(),planeShape->getPlaneNormal().getY(),planeShape->getPlaneNormal().getZ());
        //std::cerr << "N_dash = "<<glm::to_string(N_dash)<<", ";
        axis = glm::cross(N,N_dash);
        mag = glm::length(axis);
        if (mag > 0.0f) { // sanity check - no rotation may be necessary
            inv_mag = 1.0f / mag;
            axis *= inv_mag; // Normalising the axis vector
            angle = acos(glm::dot(N, N_dash) / (glm::length(N)*glm::length(N_dash)));
            //std::cerr << "Rotate ("<<angle<<", "<<glm::to_string(axis)<<")\n";
            mm = glm::rotate(mm, GLfloat(angle), axis);
        }

        // Make the plane BIIIIG
        mm = glm::scale(mm, glm::vec3(20.0, 20.0, 20.0));

        if (mmLoc != -1)
            glUniformMatrix4fv(mmLoc, 1, GL_FALSE, glm::value_ptr(mm));
        if (nmLoc != -1)
            glUniformMatrix3fv(nmLoc, 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(mm))));
        m_plane->draw(id);
        break;
    case BOX_SHAPE_PROXYTYPE:
        boxShape = dynamic_cast<btBoxShape*>(shape);

        if (boxShape == NULL) {
            std::cerr << "BodyDrawer::draw(): Critical Error: failure to dynamic cast a box shape although it's shapeType says it's ok!\n";
            exit(0);
        } else {
            extents = boxShape->getHalfExtentsWithMargin();
            mm = glm::scale(mm, glm::vec3(extents.x(), extents.y(), extents.z()));
        }
        // Set the matrices
        if (mmLoc != -1)
            glUniformMatrix4fv(mmLoc, 1, GL_FALSE, glm::value_ptr(mm));
        if (nmLoc != -1)
            glUniformMatrix3fv(nmLoc, 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(mm))));
        m_box->draw(id);
        break;
        break;
    case SPHERE_SHAPE_PROXYTYPE:
        //std::cerr <<"SPHERE_SHAPE_PROXYTYPE\n";
        // First determine the scale of our sphere by the radius
        sphereShape = dynamic_cast<btSphereShape*>(shape);
        if (sphereShape == NULL) {
            std::cerr << "BodyDrawer::draw(): Critical Error: failure to dynamic cast a sphere shape although it's shapeType says it's ok!\n";
            exit(0);
        }
        r = sphereShape->getRadius();
        mm = glm::scale(mm, glm::vec3(r,r,r));

        // Set the matrices
        if (mmLoc != -1)
            glUniformMatrix4fv(mmLoc, 1, GL_FALSE, glm::value_ptr(mm));
        if (nmLoc != -1)
            glUniformMatrix3fv(nmLoc, 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(mm))));
        m_sphere->draw(id);
        break;
    case MULTI_SPHERE_SHAPE_PROXYTYPE:
        //std::cerr <<"MULTI_SPHERE_SHAPE_PROXYTYPE\n";
        break;
    case CAPSULE_SHAPE_PROXYTYPE:
        //std::cerr <<"CAPSULE_SHAPE_PROXYTYPE\n";
        // First determine the scale of our sphere by the radius
        capsuleShape = dynamic_cast<btCapsuleShape*>(shape);

        if (capsuleShape == NULL) {
            std::cerr << "BodyDrawer::draw(): Critical Error: failure to dynamic cast a capsule shape although it's shapeType says it's ok!\n";
            exit(0);
        } else {
            r = capsuleShape->getRadius();
            h = capsuleShape->getHalfHeight();
            mm = glm::scale(mm, glm::vec3(r,h+r,r));
        }
        // Set the matrices
        if (mmLoc != -1)
            glUniformMatrix4fv(mmLoc, 1, GL_FALSE, glm::value_ptr(mm));
        if (nmLoc != -1)
            glUniformMatrix3fv(nmLoc, 1, GL_TRUE, glm::value_ptr(glm::mat3(glm::inverse(mm))));
        m_sphere->draw(id);
        break;
    case CONVEX_SHAPE_PROXYTYPE:
        //std::cerr <<"CONVEX_SHAPE_PROXYTYPE\n";
        break;
    case TRIANGLE_MESH_SHAPE_PROXYTYPE:
        //std::cerr <<"TRIANGLE_MESH_SHAPE_PROXYTYPE\n";
        break;
    case TERRAIN_SHAPE_PROXYTYPE:
        //std::cerr <<"TERRAIN_SHAPE_PROXYTYPE\n";
        break;
    default:
        std::cerr <<"unknown type: " << shape->getShapeType()<<"\n";
    }
}
