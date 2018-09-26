#ifndef TEXTUREDOBJSCENE_H
#define TEXTUREDOBJSCENE_H

#include "objscene.h"

class TexturedObjScene : public ObjScene
{
    typedef enum {
        TEXMODE_NEAREST,
        TEXMODE_BILINEAR,
        TEXMODE_TRILINEAR
    } TextureMode;
public:    
    /// Constructor from a obj mesh filename (note the default parameters)
    TexturedObjScene(const std::string &filename, const std::string &texname = "", const TextureMode & = TEXMODE_NEAREST);

    /// Destructor
    virtual ~TexturedObjScene();

    /// Initialises the scene, called before render().
    virtual void init();

    /// Called most frequently, redraws the scene.
    virtual void draw();

private:

    /// The GL texture id which is used to access and delete the texture (like a pointer)
    GLuint m_texId;

    /// Store the filename of the texture to use
    std::string m_texname;

    /// Stores the current rendered texture mode
    TextureMode m_texMode;
};

#endif // TEXTUREDOBJSCENE_H
