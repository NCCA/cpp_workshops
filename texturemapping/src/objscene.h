#ifndef OBJSCENE_H
#define OBJSCENE_H

// The base class for this object
#include "scene.h"

// Include file for Obj mesh parsing
#include <obj.hpp>

// The stl vector class
#include <vector>

/**
 * @brief The ObjScene class
 */
class ObjScene : public Scene {
public:
    /// Constructor from a obj mesh filename
    ObjScene(const std::string &filename);

    /// Destructor
    virtual ~ObjScene();

    /// Initialises the scene, called before render().
    virtual void init();

    /// Called when the window size changes (for example, if the user maximises the window).
    // virtual void resize(int w, int h);

    /// Called frequently, used if you are using some form of animation
    virtual void update();

    /// Called most frequently, redraws the scene.
    virtual void draw();

    /// All the callbacks needed to parse in the obj file
    void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);
    void geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z);
    void texture_vertex_callback(obj::float_type u, obj::float_type v);
    void vertex_normal_callback(obj::float_type nx, obj::float_type ny, obj::float_type nz);
    void triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
    void triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
    void triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
    void triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
    void quadrilateral_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3, obj::index_type v4);
    void quadrilateral_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3, const obj::index_2_tuple_type& v4_vt4);
    void quadrilateral_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3, const obj::index_2_tuple_type& v4_vn4);
    void quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3, const obj::index_3_tuple_type& v4_vt4_vn4);
    void polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
    void polygonal_face_geometric_vertices_vertex_callback(obj::index_type v);
    void polygonal_face_geometric_vertices_end_callback();
    void polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
    void polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt);
    void polygonal_face_geometric_vertices_texture_vertices_end_callback();
    void polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
    void polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn);
    void polygonal_face_geometric_vertices_vertex_normals_end_callback();
    void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
    void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn);
    void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback();
    void group_name_callback(const std::string& group_name);
    void smoothing_group_callback(obj::size_type group_number);
    void object_name_callback(const std::string& object_name);
    void material_library_callback(const std::string& filename);
    void material_name_callback(const std::string& material_name);
    void comment_callback(const std::string& comment);

private:
    std::vector<GLfloat> m_vVertexData;    //< Somewhere to store vertex data
    std::vector<GLfloat> m_fVertexData;    //< Somewhere to store face vertex data
    std::vector<GLfloat> m_vNormalData;    //< Somewhere to store vertex normal data
    std::vector<GLfloat> m_fNormalData;    //< Somewhere to store face normal data
    std::vector<GLfloat> m_vColourData;     //< The colour data goes here
    std::vector<GLfloat> m_vUVData;        //< The vertex texture coordinates go here
    std::vector<GLfloat> m_fUVData;        //< The face texture coordinates go here
    std::vector<GLuint>  m_triangleData;   //< Somewhere to store triangle face data
    std::vector<GLuint>  m_quadData;       //< Stash the quads in here

    /// The max and min bounding box extents, the center of the obj and a scaling factor for the object
    GLfloat m_maxBB[3], m_minBB[3], m_objCenter[3], m_objScale;

private:
    /// Load a mesh from the specified filename
    void load(const char *filename);

    /// The rotation of this object, used for animation
    GLfloat m_rotation;
};

#endif // OBJSCENE_H
