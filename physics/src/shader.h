#ifndef __SHADER_H
#define __SHADER_H

#include "glinc.h"

/** \brief A basic shader class. Note these calls must be made within an OpenGL context.
  *
  * Modified from the examples at http://www.swiftless.com/tutorials/glsl
  * \author Richard Southern
  * \date 3/10/2012
  */
class Shader {
public:
    /// Construct an empty shader
    Shader();

    /// Construct a shader from the two included filenames
    Shader(const char *vsFile, const char *fsFile);

    /// Destroy the shader
    ~Shader();

    bool isInit() const {return m_init;}

    /// Initialise an empty shader from the two specified filenames
    void init(const char *vsFile, const char *fsFile);
    
    /// Delete the shader so you can create a new one
    void kill();

    /// Bind the shader to the current render context
    void bind() const;

    /// Unbind the shader (revert to fixed function)
    void unbind() const;

    /// Dump the properties of this shader to the console
    void printProperties() const;

    /// Retrieve the shader id
    const GLuint &id() const {return m_id;}
    
private:
    bool m_init;       //< Is the program initialised?
    GLuint m_vp; //< The vertex shader id
    GLuint m_fp; //< The fragment shader id
    GLuint m_id; //< the program id
};

#endif //__SHADER_H



