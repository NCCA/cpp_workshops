Workshop: Comparison of Texture Mapping Techniques
--------------------------------------------------

In this workshop you'll implement various standard texture mapping strategies
and compare the result visually. 

The previous workshop has been extended to include an ObjScene class, which 
will load a standard wavefront Obj file (widely available) and display it 
by the simple Scene class framework. Note that the parser implemented here
is quite confusing, relying on an external obj parser library and you'll
probably not want to look too closely at this unless you're particularly
interested.

A new class, TexturedObjScene inherits from the ObjScene class, adding onto
the existing geometry the ability to apply a texture. The heavy lifting of
this class is done in the init() function, in which the texture is initialised
and loaded. You'll need to complete this function.

Note that I've already loaded the png data for you (using calls to libpng,
included in the pngutils functions if you're interested.

So in summary:
1) Completed the TexturedObjMesh::init() function so that it loads up the 
texture map.
2) Implement the different texture mapping methods, as specified by the 
m_texMode attribute.
3) Perform a visual comparison of these approaches.
4) Time permitting, perform a time comparison of these approaches. This may 
require designing a framework to fairly measure the rendering of the different
texturing approaches.

Good luck!
Richard Southern
21/02/2014
