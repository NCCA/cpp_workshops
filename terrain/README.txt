Terrain Generator
-----------------

For this workshop you're going to flesh out our data structure with smooth
vertex normals. A new TerrainScene class has been derived from the Scene
class which you're already familiar with. This class generates a height field
using the well known Perlin noise function (provided by an external package).
Currently the vertices and triangle indices (and vertex colour) is already
generated: you have to determine the vertex normals using this information.

The challenge is that the data is stored in an evil, C style array. This
complexity means that accessing and writing to the arrays m_data and m_tris
may be confusing, and you can expect to get unpredictable results and crash
your program frequently. Try reducing the size of the grid and using 
breakpoints, watches and debugging information (gdb) to fix it if broken.

Pointers are really important when interacting with low level functionality
of your computer, for example using drivers, OpenGL and CUDA. Also, Maya
API development depends heavily on the use of and understanding of pointers.

The ILO's for this workshop are:
- Computing of smooth vertex normals from vertex positions and triangle indices
- Pointer arithmetic!
- A peripheral bit of intuition about Perlin noise and it's applications

If you've got time, try messing around with the perlin noise parameters to
generate a new type of terrain. 

Have fun!

Richard Southern
24/03/2014
