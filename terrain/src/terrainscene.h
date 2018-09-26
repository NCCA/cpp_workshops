#ifndef TERRAINSCENE_H
#define TERRAINSCENE_H

#include "scene.h"
#include "noise.h"

/**
 * @brief The TerrainScene class
 * Construct a scene by using a Perlin noise generator. If you want to read more about Perlin noise and it's generation,
 * take a look at http://libnoise.sourceforge.net/glossary/index.html#perlinnoise .
 */
class TerrainScene : public Scene
{
public:
    /// Ctor
    TerrainScene(int xsize=256, int ysize=256);

    /// Dtor
    ~TerrainScene();

    /// Initialises the scene, called before render().
    virtual void init();

    /// Called frequently, used if you are using some form of animation
    virtual void update();

    virtual void resize(int w, int h);

    /// Called most frequently, redraws the scene.
    virtual void draw();

private:
    /// A Perlin noise generator
    noise::module::Perlin m_noise;

    /// A chunk of data
    GLfloat *m_data;

    /// The triangle indices of our terrain
    GLuint *m_tris;

    /// The current rotation (so it'll spin)
    GLfloat m_rotation;

    /// The size of the terrain in x,y and total number of vertices and triangles
    GLuint m_nVerts, m_nTris, m_xsize, m_ysize;
};

#endif // TERRAINSCENE_H
