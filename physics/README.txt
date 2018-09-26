Create a physics scene in Bullet
--------------------------------
For this workshop you'll get to experiment with the library Bullet.
Essentially you will create your own subclass of the Scene class (say,
MyScene) and then in the constructor build a physics scene. Note that
the rigid bodies and constraints are managed by the Scene in the m_bodies
and m_constraints vectors respectively, so you'll need to add them there.

For more information on Bullet, examples and tutorials, take a look at
http://bulletphysics.org/wordpress/, click on the link to Documentation
at the top.

I have included two example scenes, the TestScene (very simple) and the
PassiveWalkerScene (considerably more complex! also doesn't walk :( ) which
is something I'm working on for research purposes. Look at the TestScene
before trying to figure out the PassiveWalker!

The viewer which I have implemented (research code, so potentially buggy!)
supports the display of BoxShapes, SphereShapes, CapsuleShapes and 
StaticPlanes, so this should be enough to get you started.

If you need something specific to do, try and build a 3D Angry Birds 
scene, i.e. stack some blocks and fire a sphere into them, then watch
the chaos!

Note that this workshop will heavily test your understanding of an API,
and your ability to distil concepts from existing code: a valuable skill
in the industrial setting. Have fun!

Richard Southern
10/03/2014
