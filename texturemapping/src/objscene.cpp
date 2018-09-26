#include "objscene.h"

// The size increment when we run out of space in our vectors
#define VECTOR_SIZE_INCREMENT 64
// needed for sqrt
#include <math.h>
// needed for cout / cerr
#include <iostream>
// Use this to keep track of the times
#include <sys/time.h>

/// This namespace makes it easier to access the placeholder name for tr1 calls
//using namespace std::tr1::placeholders;   // for _1, _2, etc.
namespace pl=std::tr1::placeholders;

ObjScene::ObjScene(const std::string &filename) : Scene(), m_rotation(0.0f)
{
    m_vVertexData.reserve(VECTOR_SIZE_INCREMENT);
    m_fVertexData.reserve(VECTOR_SIZE_INCREMENT);
    m_vNormalData.reserve(VECTOR_SIZE_INCREMENT);
    m_fNormalData.reserve(VECTOR_SIZE_INCREMENT);
    m_vColourData.reserve(VECTOR_SIZE_INCREMENT);
    m_vUVData.reserve(VECTOR_SIZE_INCREMENT);
    m_fUVData.reserve(VECTOR_SIZE_INCREMENT);

    // Used for indexed vectors
    m_triangleData.reserve(VECTOR_SIZE_INCREMENT);
    m_quadData.reserve(VECTOR_SIZE_INCREMENT);

    if (!filename.empty()) {
        load(filename.c_str());
    }

    // Default is to draw elements
   // m_useElements = true;
}

/**
 * @brief ObjScene::~ObjScene
 */
ObjScene::~ObjScene() {
    // Note that vectors will neatly kill themselves when they are out of scope
}

/**
 * @brief ObjScene::init
 */
void ObjScene::init() {
    // Call the parent class initialise routine
    Scene::init();

    // Enable material properties for our mesh
    GLfloat matAmb[] = {0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat matDiff[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat matSpec[] = {0.0f, 1.0f, 0.0f, 1.0f};
    GLfloat matShine = 1000.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShine);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

/**
 * @brief ObjScene::draw Draws the Obj data using OpenGL.
 */
void ObjScene::draw() {
    if (!m_isInit) return;

    // Clear the display buffer (colour set by glClearColor)
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Set our scene transformations
    glMatrixMode(GL_MODELVIEW); // set the modelview matrix here
    glLoadIdentity(); // Set it to the identity (no transformations
    //glTranslatef(m_objCenter[0], m_objCenter[1], m_objCenter[2]);
    glTranslatef(0.0f,0.0f,-1.0f);
    glScalef(m_objScale,m_objScale,m_objScale);
    glRotatef(m_rotation, 1.0f, 0.0f, 0.0f); // Rotate about the y axis (try swapping this with the line above!)
    glTranslatef(-m_objCenter[0], -m_objCenter[1], -m_objCenter[2]);

    //glFrontFace(GL_CCW); // Use counterclockwise face ordering (obj default)

    if (m_fVertexData.size() == 0) {
        if (m_vVertexData.size() > 0) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, (GLfloat*) &(m_vVertexData[0]));
        }
        if (m_vNormalData.size() > 0) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, (GLfloat*) &(m_vNormalData[0]));
        }
        if (m_vColourData.size() > 0) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, (GLfloat*) &(m_vColourData[0]));
        }
        if (m_vUVData.size() > 0) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, (GLfloat*) &(m_vUVData[0]));
        } else {
            glDisable(GL_TEXTURE_2D);
        }
        // Draw the triangles
        if (m_triangleData.size() > 0) {
            glDrawElements(GL_TRIANGLES, m_triangleData.size(), GL_UNSIGNED_INT, (GLuint*) &(m_triangleData[0]));
        } else if (m_quadData.size() > 0) {
            glDrawElements(GL_QUADS, m_quadData.size(), GL_UNSIGNED_INT, (GLuint*) &(m_quadData[0]));
        }

        // Disable the enabled client states
        if (m_vVertexData.size() > 0) glDisableClientState(GL_VERTEX_ARRAY);
        if (m_vNormalData.size() > 0) glDisableClientState(GL_NORMAL_ARRAY);
        if (m_vColourData.size() > 0) glDisableClientState(GL_COLOR_ARRAY);
        if (m_vUVData.size() > 0) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    } else {
        // Here we draw the data in arrays
        if (m_fVertexData.size() > 0) {
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, (GLfloat*) &(m_fVertexData[0]));
        }
        if (m_fNormalData.size() > 0) {
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, 0, (GLfloat*) &(m_fNormalData[0]));
        }
        /*
        if (m_fColourData.size() > 0) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(3, GL_FLOAT, 0, (GLfloat*) &(m_fColourData[0]));
        }*/
        if (m_fUVData.size() > 0) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, (GLfloat*) &(m_fUVData[0]));
        } else {
            glDisable(GL_TEXTURE_2D);
        }

        // Draw the triangles
        if (m_triangleData.size() > 0) {
            glDrawArrays(GL_TRIANGLES, 0, m_fVertexData.size()/3);
        } else if (m_quadData.size() > 0) {
            glDrawArrays(GL_QUADS, 0, m_fVertexData.size()/3);
        }

        // Disable the enabled client states
        if (m_fVertexData.size() > 0) glDisableClientState(GL_VERTEX_ARRAY);
        if (m_fNormalData.size() > 0) glDisableClientState(GL_NORMAL_ARRAY);
        if (m_fUVData.size() > 0) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

void ObjScene::update() {
    Scene::update();

    // Use the elapsed time to do some sort of animation
    m_rotation = GLfloat(m_elapsedTime) * 100.0f;
}

/*******************************************************************************
 *
 * Below are all the callbacks and functions for file parsing - trying to
 * understand this will be painful!
 *
 ******************************************************************************/
/**
 * @brief ObjScene::load (called from constructor usually)
 * @param filename The filename to use
 */
void ObjScene::load(const char* filename) {
    //obj_parser.geometric_vertex_callback(std::tr1::bind(&ObjScene::geometric_vertex_callback, this, _1, _2, _3));
    //obj_parser.vertex_normal_callback(std::tr1::bind(&ObjScene::geometric_normal_callback, this, _1, _2, _3));

    obj::obj_parser obj_parser(obj::obj_parser::parse_blank_lines_as_comment);

    obj_parser.info_callback(std::tr1::bind(&ObjScene::info_callback, this, filename, pl::_1, pl::_2));
    obj_parser.warning_callback(std::tr1::bind(&ObjScene::warning_callback, this, filename, pl::_1, pl::_2));
    obj_parser.error_callback(std::tr1::bind(&ObjScene::error_callback, this, filename, pl::_1, pl::_2));

    obj_parser.geometric_vertex_callback(std::tr1::bind(&ObjScene::geometric_vertex_callback, this, pl::_1, pl::_2, pl::_3));
    obj_parser.texture_vertex_callback(std::tr1::bind(&ObjScene::texture_vertex_callback, this, pl::_1, pl::_2));
    obj_parser.vertex_normal_callback(std::tr1::bind(&ObjScene::vertex_normal_callback, this, pl::_1, pl::_2, pl::_3));
    obj_parser.face_callbacks(
                std::tr1::bind(&ObjScene::triangular_face_geometric_vertices_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::triangular_face_geometric_vertices_texture_vertices_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::triangular_face_geometric_vertices_vertex_normals_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::quadrilateral_face_geometric_vertices_callback, this, pl::_1, pl::_2, pl::_3, pl::_4),
                std::tr1::bind(&ObjScene::quadrilateral_face_geometric_vertices_texture_vertices_callback, this, pl::_1, pl::_2, pl::_3, pl::_4),
                std::tr1::bind(&ObjScene::quadrilateral_face_geometric_vertices_vertex_normals_callback, this, pl::_1, pl::_2, pl::_3, pl::_4),
                std::tr1::bind(&ObjScene::quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, pl::_1, pl::_2, pl::_3, pl::_4),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_begin_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_vertex_callback, this, pl::_1),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_end_callback, this),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_texture_vertices_begin_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_callback, this, pl::_1),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_texture_vertices_end_callback, this),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_vertex_normals_begin_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_vertex_normals_vertex_callback, this, pl::_1),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_vertex_normals_end_callback, this),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, this, pl::_1, pl::_2, pl::_3),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, this, pl::_1),
                std::tr1::bind(&ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback, this)
                );
    obj_parser.group_name_callback(std::tr1::bind(&ObjScene::group_name_callback, this, pl::_1));
    obj_parser.smoothing_group_callback(std::tr1::bind(&ObjScene::smoothing_group_callback, this, pl::_1));
    obj_parser.object_name_callback(std::tr1::bind(&ObjScene::object_name_callback, this, pl::_1));
    obj_parser.material_library_callback(std::tr1::bind(&ObjScene::material_library_callback, this, pl::_1));
    obj_parser.material_name_callback(std::tr1::bind(&ObjScene::material_name_callback, this, pl::_1));
    obj_parser.comment_callback(std::tr1::bind(&ObjScene::comment_callback, this, pl::_1));

    obj_parser.parse(filename);

    // Create a color array if there wasn't one
    uint i,j;
    if (m_vColourData.size() == 0) {
        m_vColourData.resize(m_vVertexData.size());
        for (i=0; i<m_vVertexData.size(); ++i) {
            m_vColourData[i] = 1.0f;
        }
    }

    // Determine the bounds of the obj mesh (note that it is stored in x,y,z,x,y,z,... format
    for (i=0; i<m_vVertexData.size(); i += 3) {
        if (i==0) {
            for (j = 0; j < 3; ++j)
                m_maxBB[j] = m_minBB[j] = m_vVertexData[i+j];
        } else {
            for (j=0; j < 3; ++j) {
                if (m_maxBB[j] < m_vVertexData[i+j]) {
                    m_maxBB[j] = m_vVertexData[i+j];
                }
                if (m_minBB[j] > m_vVertexData[i+j])
                    m_minBB[j] = m_vVertexData[i+j];
            }
        }
    }

    // Now we have the bounds of the object we can compute the translation necessary to see the object
    // We're going to use the bounding box diagonal for the distance.
    m_objScale = 0.0f;

    for (j = 0; j < 3; ++j) {
        m_objScale += (m_maxBB[j]-m_minBB[j]) * (m_maxBB[j]-m_minBB[j]);
        m_objCenter[j] = 0.5f * (m_maxBB[j] + m_minBB[j]);
    }
    if (m_objScale != 0.0f) // Always good to have a sanity check
        m_objScale = 1.0f / sqrt(m_objScale);
}

/**
  * Append vertex data which is read from the file. i.e. v in the obj file.
  */
void ObjScene::geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z) {
    // Check the size of the vector can handle another 3 elements
    if ((m_vVertexData.size() + 3) > m_vVertexData.capacity()) {
        m_vVertexData.reserve(m_vVertexData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_vVertexData.push_back(x); m_vVertexData.push_back(y); m_vVertexData.push_back(z);
}

/**
  * Vertex normal data (i.e. vn in the obj file)
  */
void ObjScene::vertex_normal_callback(obj::float_type nx, obj::float_type ny, obj::float_type nz) {
    if ((m_vNormalData.size() + 3) > m_vNormalData.capacity()) {
        m_vNormalData.reserve(m_vNormalData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    float inv_sqrt_mag = 1.0f / sqrt(nx*nx + ny*ny + nz*nz);
    float nnx = nx * inv_sqrt_mag;
    float nny = ny * inv_sqrt_mag;
    float nnz = nz * inv_sqrt_mag;
    m_vNormalData.push_back(nnx); m_vNormalData.push_back(nny); m_vNormalData.push_back(nnz);
}

/**
  * Triangle data without texture or normal indices
  */
void ObjScene::triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3) {
    if ((m_triangleData.size() + 3) > m_triangleData.capacity()) {
        m_triangleData.reserve(m_triangleData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_triangleData.push_back(v1-1); m_triangleData.push_back(v2-1); m_triangleData.push_back(v3-1);
}

/**
  * Quad face indices without texture or normal indices.
  */
void ObjScene::quadrilateral_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3, obj::index_type v4) {
    if ((m_quadData.size() + 4) > m_quadData.capacity()) {
        m_quadData.reserve(m_quadData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_quadData.push_back(v1-1); m_quadData.push_back(v2-1); m_quadData.push_back(v3-1); m_quadData.push_back(v4-1);
}


void ObjScene::info_callback(const std::string& filename, std::size_t line_number, const std::string& message) {
    std::cerr << filename << ":" << line_number << ": " << "info: " << message << std::endl;
}

void ObjScene::warning_callback(const std::string& filename, std::size_t line_number, const std::string& message) {
    std::cerr << filename << ":" << line_number << ": " << "warning: " << message << std::endl;
}

void ObjScene::error_callback(const std::string& filename, std::size_t line_number, const std::string& message) {
    std::cerr << filename << ":" << line_number << ": " << "error: " << message << std::endl;
}

/**
  * Texture coords, should be one per vertex we hope.
  */
void ObjScene::texture_vertex_callback(obj::float_type u, obj::float_type v) {
    if ((m_vUVData.size() + 2) > m_vUVData.capacity()) {
        m_vUVData.reserve(m_vUVData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_vUVData.push_back(u);
    m_vUVData.push_back(v);
}

/**
  * Face indices with both vertex and uv texture coords, i.e. f/t in the obj file
  */
void ObjScene::triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3) {
    std::cout << "f " << std::tr1::get<0>(v1_vt1) << "/" << std::tr1::get<1>(v1_vt1) << " " << std::tr1::get<0>(v2_vt2) << "/" << std::tr1::get<1>(v2_vt2) << " " << std::tr1::get<0>(v3_vt3) << "/" << std::tr1::get<1>(v3_vt3) << "\n";
    if ((m_triangleData.size() + 3) > m_triangleData.capacity()) {
        m_triangleData.reserve(m_triangleData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_triangleData.push_back(std::tr1::get<0>(v1_vt1)-1);
    m_triangleData.push_back(std::tr1::get<0>(v2_vt2)-1);
    m_triangleData.push_back(std::tr1::get<0>(v3_vt3)-1);

    // Retrieve the first vertex texture coordinate
    if ((m_fUVData.size() + 6) > m_fUVData.capacity())
        m_fUVData.reserve(m_fUVData.capacity() + VECTOR_SIZE_INCREMENT);

    // Now create the face vertex Vertexs
    if ((m_fVertexData.size() + 9) > m_fVertexData.capacity())
        m_fVertexData.reserve(m_fVertexData.capacity() + VECTOR_SIZE_INCREMENT);

    int idx;

    idx = std::tr1::get<0>(v1_vt1)-1; // Find the index of the first vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v2_vt2)-1; // Find the index of the second vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v3_vt3)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<1>(v1_vt1)-1; // Find the index of the first vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v2_vt2)-1; // Find the index of the second vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v3_vt3)-1; // Find the index of the third vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);
}

/**
  * Face indices based on vertex and face normals, i.e. the face data looks like f//n in the obj file.
  */
void ObjScene::triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3) {
    // First create the triangle data
    if ((m_triangleData.size() + 3) > m_triangleData.capacity())
        m_triangleData.reserve(m_triangleData.capacity() + VECTOR_SIZE_INCREMENT);

    m_triangleData.push_back(std::tr1::get<0>(v1_vn1)-1);
    m_triangleData.push_back(std::tr1::get<0>(v2_vn2)-1);
    m_triangleData.push_back(std::tr1::get<0>(v3_vn3)-1);

    int idx;

    // Now create the face vertices
    if ((m_fVertexData.size() + 9) > m_fVertexData.capacity())
        m_fVertexData.reserve(m_fVertexData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<0>(v1_vn1)-1; // Find the index of the first vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v2_vn2)-1; // Find the index of the second vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v3_vn3)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    // Now create the face vertex normals
    if ((m_fNormalData.size() + 9) > m_fNormalData.capacity())
        m_fNormalData.reserve(m_fNormalData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<1>(v1_vn1)-1; // Find the index of the first vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<1>(v2_vn2)-1; // Find the index of the second vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<1>(v3_vn3)-1; // Find the index of the third vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);
}

/**
  * Face indices with both uv indices and normal indices, e.g. f/t/n.
  */
void ObjScene::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3) {
    // First create the triangle data
    if ((m_triangleData.size() + 3) > m_triangleData.capacity()) {
        m_triangleData.reserve(m_triangleData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_triangleData.push_back(std::tr1::get<0>(v1_vt1_vn1)-1);
    m_triangleData.push_back(std::tr1::get<0>(v2_vt2_vn2)-1);
    m_triangleData.push_back(std::tr1::get<0>(v3_vt3_vn3)-1);

    int idx;

    // Now create the face vertex Vertexs
    if ((m_fVertexData.size() + 9) > m_fVertexData.capacity())
        m_fVertexData.reserve(m_fVertexData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<0>(v1_vt1_vn1)-1; // Find the index of the first vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v2_vt2_vn2)-1; // Find the index of the second vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v3_vt3_vn3)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    // Retrieve the first vertex texture coordinate
    if ((m_fUVData.size() + 6) > m_fUVData.capacity())
        m_fUVData.reserve(m_fUVData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<1>(v1_vt1_vn1)-1; // Find the index of the first vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v2_vt2_vn2)-1; // Find the index of the second vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v3_vt3_vn3)-1; // Find the index of the third vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    // Now create the face vertex normals
    if ((m_fNormalData.size() + 9) > m_fNormalData.capacity())
        m_fNormalData.reserve(m_fNormalData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<2>(v1_vt1_vn1)-1; // Find the index of the first vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<2>(v2_vt2_vn2)-1; // Find the index of the second vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<2>(v3_vt3_vn3)-1; // Find the index of the third vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);
}

void ObjScene::quadrilateral_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3, const obj::index_2_tuple_type& v4_vt4) {
    //cout << "f " << std::tr1::get<0>(v1_vt1) << "/" << std::tr1::get<1>(v1_vt1) << " " << std::tr1::get<0>(v2_vt2) << "/" << std::tr1::get<1>(v2_vt2) << " " << std::tr1::get<0>(v3_vt3) << "/" << std::tr1::get<1>(v3_vt3) << " " << std::tr1::get<0>(v4_vt4) << "/" << std::tr1::get<1>(v4_vt4) << "\n";
    if ((m_quadData.size() + 4) > m_quadData.capacity()) {
        m_quadData.reserve(m_quadData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_quadData.push_back(std::tr1::get<0>(v1_vt1)-1);
    m_quadData.push_back(std::tr1::get<0>(v2_vt2)-1);
    m_quadData.push_back(std::tr1::get<0>(v3_vt3)-1);
    m_quadData.push_back(std::tr1::get<0>(v4_vt4)-1);

    // Retrieve the first vertex texture coordinate
    if ((m_fUVData.size() + 8) > m_fUVData.capacity())
        m_fUVData.reserve(m_fUVData.capacity() + VECTOR_SIZE_INCREMENT);

    int idx;

    // Now create the face vertex Vertexs
    if ((m_fVertexData.size() + 12) > m_fVertexData.capacity())
        m_fVertexData.reserve(m_fVertexData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<0>(v1_vt1)-1; // Find the index of the first vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v2_vt2)-1; // Find the index of the second vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v3_vt3)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v4_vt4)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<1>(v1_vt1)-1; // Find the index of the first vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v2_vt2)-1; // Find the index of the second vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v3_vt3)-1; // Find the index of the third vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v4_vt4)-1; // Find the index of the third vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);
}

void ObjScene::quadrilateral_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3, const obj::index_2_tuple_type& v4_vn4) {
    //cout << "f " << std::tr1::get<0>(v1_vn1) << "//" << std::tr1::get<1>(v1_vn1) << " " << std::tr1::get<0>(v2_vn2) << "//" << std::tr1::get<1>(v2_vn2) << " " << std::tr1::get<0>(v3_vn3) << "//" << std::tr1::get<1>(v3_vn3) << " " << std::tr1::get<0>(v4_vn4) << "//" << std::tr1::get<1>(v4_vn4) << "\n";
    if ((m_quadData.size() + 4) > m_quadData.capacity()) {
        m_quadData.reserve(m_quadData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_quadData.push_back(std::tr1::get<0>(v1_vn1)-1);
    m_quadData.push_back(std::tr1::get<0>(v2_vn2)-1);
    m_quadData.push_back(std::tr1::get<0>(v3_vn3)-1);
    m_quadData.push_back(std::tr1::get<0>(v4_vn4)-1);

    int idx;

    // Now create the face vertex Vertexs
    if ((m_fVertexData.size() + 12) > m_fVertexData.capacity())
        m_fVertexData.reserve(m_fVertexData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<0>(v1_vn1)-1; // Find the index of the first vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v2_vn2)-1; // Find the index of the second vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v3_vn3)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v4_vn4)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    // Now create the face vertex normals
    if ((m_fNormalData.size() + 12) > m_fNormalData.capacity())
        m_fNormalData.reserve(m_fNormalData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<1>(v1_vn1)-1; // Find the index of the first vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<1>(v2_vn2)-1; // Find the index of the second vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<1>(v3_vn3)-1; // Find the index of the third vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<1>(v4_vn4)-1; // Find the index of the third vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);
}

void ObjScene::quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3, const obj::index_3_tuple_type& v4_vt4_vn4) {
    //cout << "f " << std::tr1::get<0>(v1_vt1_vn1) << "/" << std::tr1::get<1>(v1_vt1_vn1) << "/" << std::tr1::get<2>(v1_vt1_vn1) << " " << std::tr1::get<0>(v2_vt2_vn2) << "/" << std::tr1::get<1>(v2_vt2_vn2) << "/" << std::tr1::get<2>(v2_vt2_vn2) << " " << std::tr1::get<0>(v3_vt3_vn3) << "/" << std::tr1::get<1>(v3_vt3_vn3) << "/" << std::tr1::get<2>(v3_vt3_vn3) << " " << std::tr1::get<0>(v4_vt4_vn4) << "/" << std::tr1::get<1>(v4_vt4_vn4) << "/" << std::tr1::get<2>(v4_vt4_vn4) << "\n";
    if ((m_quadData.size() + 4) > m_quadData.capacity()) {
        m_quadData.reserve(m_quadData.capacity() + VECTOR_SIZE_INCREMENT);
    }
    m_quadData.push_back(std::tr1::get<0>(v1_vt1_vn1)-1);
    m_quadData.push_back(std::tr1::get<0>(v2_vt2_vn2)-1);
    m_quadData.push_back(std::tr1::get<0>(v3_vt3_vn3)-1);
    m_quadData.push_back(std::tr1::get<0>(v4_vt4_vn4)-1);

    int idx;

    // Retrieve the first vertex texture coordinate
    if ((m_fUVData.size() + 8) > m_fUVData.capacity())
        m_fUVData.reserve(m_fUVData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<1>(v1_vt1_vn1)-1; // Find the index of the first vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v2_vt2_vn2)-1; // Find the index of the second vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v3_vt3_vn3)-1; // Find the index of the third vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    idx = std::tr1::get<1>(v4_vt4_vn4)-1; // Find the index of the third vertex uv
    m_fUVData.push_back(m_vUVData[idx*2 + 0]); // Assign the values to the face uv data
    m_fUVData.push_back(m_vUVData[idx*2 + 1]);

    // Now create the face vertex Vertexs
    if ((m_fVertexData.size() + 12) > m_fVertexData.capacity())
        m_fVertexData.reserve(m_fVertexData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<0>(v1_vt1_vn1)-1; // Find the index of the first vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v2_vt2_vn2)-1; // Find the index of the second vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v3_vt3_vn3)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    idx = std::tr1::get<0>(v4_vt4_vn4)-1; // Find the index of the third vertex Vertex
    m_fVertexData.push_back(m_vVertexData[idx*3 + 0]); // Assign the values to the face Vertex data
    m_fVertexData.push_back(m_vVertexData[idx*3 + 1]);
    m_fVertexData.push_back(m_vVertexData[idx*3 + 2]);

    // Now create the face vertex normals
    if ((m_fNormalData.size() + 12) > m_fNormalData.capacity())
        m_fNormalData.reserve(m_fNormalData.capacity() + VECTOR_SIZE_INCREMENT);

    idx = std::tr1::get<2>(v1_vt1_vn1)-1; // Find the index of the first vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<2>(v2_vt2_vn2)-1; // Find the index of the second vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<2>(v3_vt3_vn3)-1; // Find the index of the third vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);

    idx = std::tr1::get<2>(v4_vt4_vn4)-1; // Find the index of the third vertex normal
    m_fNormalData.push_back(m_vNormalData[idx*3 + 0]); // Assign the values to the face normal data
    m_fNormalData.push_back(m_vNormalData[idx*3 + 1]);
    m_fNormalData.push_back(m_vNormalData[idx*3 + 2]);
}

/**
  * Polygonal faces need to be triangulated, which is tedious. Any mesh can be triangulated in Meshlab before import.
  */
void ObjScene::polygonal_face_geometric_vertices_begin_callback(obj::index_type /*v1*/, obj::index_type /*v2*/, obj::index_type /*v3*/) {
    //cout << "f " << v1 << " " << v2 << " " << v3;
}

void ObjScene::polygonal_face_geometric_vertices_vertex_callback(obj::index_type /*v*/) {
    //cout << " " << v;
}

void ObjScene::polygonal_face_geometric_vertices_end_callback() {
    //cout << "\n";
}

void ObjScene::polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& /*v1_vt1*/, const obj::index_2_tuple_type& /*v2_vt2*/, const obj::index_2_tuple_type& /*v3_vt3*/) {
    //(*ostream_) << "f " << std::tr1::get<0>(v1_vt1) << "/" << std::tr1::get<1>(v1_vt1) << " " << std::tr1::get<0>(v2_vt2) << "/" << std::tr1::get<1>(v2_vt2) << " " << std::tr1::get<0>(v3_vt3) << "/" << std::tr1::get<1>(v3_vt3);
}

void ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& /*v_vt*/) {
    //(*ostream_) << " " << std::tr1::get<0>(v_vt) << "/" << std::tr1::get<1>(v_vt);
}

void ObjScene::polygonal_face_geometric_vertices_texture_vertices_end_callback() {
    //(*ostream_) << "\n";
}

void ObjScene::polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& /*v1_vn1*/, const obj::index_2_tuple_type& /*v2_vn2*/, const obj::index_2_tuple_type& /*v3_vn3*/) {
    //(*ostream_) << "f " << std::tr1::get<0>(v1_vn1) << "//" << std::tr1::get<1>(v1_vn1) << " " << std::tr1::get<0>(v2_vn2) << "//" << std::tr1::get<1>(v2_vn2) << " " << std::tr1::get<0>(v3_vn3) << "//" << std::tr1::get<1>(v3_vn3);
}

void ObjScene::polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& /*v_vn*/) {
    //(*ostream_) << " " << std::tr1::get<0>(v_vn) << "//" << std::tr1::get<1>(v_vn);
}

void ObjScene::polygonal_face_geometric_vertices_vertex_normals_end_callback() {
    //(*ostream_) << "\n";
}

void ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& /*v1_vt1_vn1*/, const obj::index_3_tuple_type& /*v2_vt2_vn2*/, const obj::index_3_tuple_type& /*v3_vt3_vn3*/) {
    //(*ostream_) << "f " << std::tr1::get<0>(v1_vt1_vn1) << "/" << std::tr1::get<1>(v1_vt1_vn1) << "/" << std::tr1::get<2>(v1_vt1_vn1) << " " << std::tr1::get<0>(v2_vt2_vn2) << "/" << std::tr1::get<1>(v2_vt2_vn2) << "/" << std::tr1::get<2>(v2_vt2_vn2) << " " << std::tr1::get<0>(v3_vt3_vn3) << "/" << std::tr1::get<1>(v3_vt3_vn3) << "/" << std::tr1::get<2>(v3_vt3_vn3);
}

void ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& /*v_vt_vn*/) {
    //(*ostream_) << " " << std::tr1::get<0>(v_vt_vn) << "/" << std::tr1::get<1>(v_vt_vn) << "/" << std::tr1::get<2>(v_vt_vn);
}

void ObjScene::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback() {
    //(*ostream_) << "\n";
}

void ObjScene::group_name_callback(const std::string& /*group_name*/) {
    //(*ostream_) << "g " << group_name << "\n";
}

void ObjScene::smoothing_group_callback(obj::size_type /*group_number*/) {
    /*
  (*ostream_) << "s ";
  if (group_number == 0) {
    (*ostream_) << "off";
  }
  else {
    (*ostream_) << group_number;
  }
  (*ostream_) << "\n";*/
}

void ObjScene::object_name_callback(const std::string& /*object_name*/) {
    //(*ostream_) << "o " << object_name << "\n";
}

void ObjScene::material_library_callback(const std::string& /*filename*/) {
    //(*ostream_) << "mtllib " << filename << "\n";
}

void ObjScene::material_name_callback(const std::string& /*material_name*/) {
    //(*ostream_) << "usemtl " << material_name << "\n";
}

void ObjScene::comment_callback(const std::string& /*comment*/) {
    //(*ostream_) << comment << "\n";
}
