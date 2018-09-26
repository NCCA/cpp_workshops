#ifndef RENDEROBJECTS_H
#define RENDEROBJECTS_H

#include "glinc.h"
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>

/**
 * @brief The RenderObject class
 */
class RenderObject {
public:
    /// Creator
    RenderObject();

    /// Destroyer
    virtual ~RenderObject() {
        if (m_init) {
            glDeleteBuffers(1, &m_vbo);
            glDeleteBuffers(1, &m_nbo);
            glDeleteBuffers(1, &m_tcbo);
            glDeleteBuffers(1, &m_eabo);
            glDeleteBuffers(1, &m_cbo);
            glDeleteVertexArrays(1, &m_va);
        }
    }

    /// Create an object of this type
    virtual void init() = 0;

    /// Draw the render object using standard shader parameters
    virtual void draw(GLuint /*shader id*/);

    /// Attribute retrieval
    const GLuint &getVertexArray() {return m_va;}
    const GLuint &getVertexArrayObject() {return m_vbo;}
    const GLuint &getNormalArrayObject() {return m_nbo;}
    const GLuint &getTextureCoordObject() {return m_tcbo;}
    const GLuint &getElementArrayObject() {return m_eabo;}
    const GLuint &getVertexCount() {return m_nverts;}
    const GLuint &getTriangleCount() {return m_ntris;}

protected:
    bool m_init; //< Keep track of whether this object has been initialised
    GLuint m_va; //< Vertex Array
    GLuint m_vbo; //< Vertex Array Buffer Object
    GLuint m_nbo; //< Normal Array Buffer Object
    GLuint m_cbo; //< Colour buffer object
    GLuint m_tcbo; //< Texture Coord Array Buffer Object
    GLuint m_eabo; //< Element Array Buffer Object
    GLuint m_nverts, m_ntris; //< Number of vertices / triangles respectively
    glm::vec3 m_diffuse; //< Allows us to specify alternative colours for objects
};

/**
 * @brief The PlaneRenderObject class
 */
class PlaneRenderObject : public RenderObject {
public:
    /// Create an empty plane
    PlaneRenderObject() : RenderObject() {}

    /// Destroy the plane
    virtual ~PlaneRenderObject() {}

    /// Create the buffers and data associated with this object
    void init();
};

/**
 * @brief The SphereRenderObject class
 */
class SphereRenderObject : public RenderObject {
public:
    /// Create an empty plane
    SphereRenderObject() : RenderObject() {}

    /// Destroy the plane
    virtual ~SphereRenderObject() {}

    /// Create the buffers and data associated with this object
    void init();
};

/**
 * @brief The BoxRenderObject class
 */
class BoxRenderObject : public RenderObject {
public:
    /// Create the Box object
    BoxRenderObject() : RenderObject() {}

    /// Destroy the box
    virtual ~BoxRenderObject() {}

    /// Create the buffers and data associated with this object
    void init();
};

/**
 * @brief The AxisRenderObject class
 */
class AxisRenderObject : public RenderObject {
public:
    /// Create an empty object
    AxisRenderObject() : RenderObject() {}
    virtual ~AxisRenderObject() {}
    virtual void draw(GLuint);
    void init();
};

#endif // RENDEROBJECTS_H
