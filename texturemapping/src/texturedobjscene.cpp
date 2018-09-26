#include "texturedobjscene.h"

// Needed for texture loading
#include "pngutils.h"
#include <iostream>
#include <QImage>
#include <QtOpenGL/QGLWidget>

/**
 * @brief TexturedObjScene::TexturedObjScene constructs the scene
 * @param _filename The filename of the obj mesh to load (handled by parent class)
 * @param _texname The filename of the texture (if empty, no texture will be mapped)
 * @param _texMode The mode of the texture (an enum in the class header)
 */
TexturedObjScene::TexturedObjScene(const std::string &_filename, const std::string &_texname, const TextureMode &_texMode)
    : ObjScene(_filename), m_texname(_texname), m_texMode(_texMode)
{
}

/**
 * @brief TexturedObjScene::~TexturedObjScene delete allocated memory (for e.g. the texture)
 */
TexturedObjScene::~TexturedObjScene()  {
    if (m_isInit && (!m_texname.empty())) {
        // Delete the memory allocated for the texture here
        glDeleteTextures(1, &m_texId);
    }
}

/**
 * @brief TexturedObjScene::init initialise the texture if one is specified.
 */
void TexturedObjScene::init() {
    // Call the parent class initialiser
    ObjScene::init();

    // Don't initialise the texture if the user didn't specify one
    if(m_texname.empty()) return;

    // Load up the data from the image using the Qt image functions
    QImage texImage = QImage(m_texname.c_str());
    QImage texData = QGLWidget::convertToGLFormat(texImage);
    // Note that texData will contain all the goodies you need to create the texture.
    // Take a look at http://doc.qt.io/qt-4.8/qglwidget.html#convertToGLFormat
    // for how this might be used with GL calls. Note that the internal and external formats
    // are GL_RGBA.


    //*** Initialise the texturing here
    //*** You'll need to consider the m_texMode parameter to support different texture mapping techniques
    //*** as well as initialising a mipmap where necessary.
    //*** Take a look at glGenTextures(), glBindTexture(), glTexImage2D(), glTexParameterf()


}

/**
 * @brief TexturedObjScene::draw draws the current scene (nothing special needed for the texture - everything is bound in init())
 */
void TexturedObjScene::draw() {
    // Call the parent class draw routine
    ObjScene::draw();
}

