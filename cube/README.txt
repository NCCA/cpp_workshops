Workshop: Make a spinning cube!
-------------------------------

In this workshop you'll get to demonstrate your understanding of some basic
OpenGL concepts using Immediate mode rendering to display a spinning cube.

This will involve the following elements in roughly this order:

1) Create a new class derived from the Scene class. You might want to call
it CubeScene.

2) Overload the init() function in CubeScene to create the data for displaying
your cube. This can be stored and initialised in a number of ways.

3) Overload the draw() function in CubeScene to display the data that you
created in init(). This should be something within a glBegin(GL_TRIANGLES)/
glEnd() pair, consisting of calls to glVertex3f and any other appropriate
functions. 

4) Finally, make your model animate by overloading the update function. Note
that the if you call the parent class Scene::update() it will update the
attribute m_elapsedTime which contains the time since the program started
running in seconds.

Don't try (4) until you've done the other 3!

Good luck!
Richard Southern
20/02/2014
