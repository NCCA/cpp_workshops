Extend the Canvas Application
-----------------------------

Consider the scenario. In your new job, you're given this code and you've 
been instructed to implement a number of new features. The application 
is a basic SDL canvas application - by dragging and dropping the mouse 
the user can create new primitives, like lines, rectangles and circles. 
Currently only lines and rectangles are implemented.

The new features your boss requires are listed in increasing order of 
difficulty (as he/she sees it):
- Adding the ability to cycle between the available operations by pushing
  'o' - this will only affect the next operation drawn. Currently LineOperation
  and RectangleOperation are provided (feel free to implement others).
- Do the same for the colour of the operation, which can be cycled by
  pushing 'c'.
- Implement rubber banding - the ability to preview the shape of the primite
  before the mouse button is released. This can be implemented by modifying
  the existing mouseUp, mouseDown and mouseMove.
- Your boss has now decided in her wisdom to require the implementation
  of that classic magical operation used in countless applications: the
  ability to undo and redo. To recap, this means that the last operation
  will be "removed", although it can be restored by performing the redo
  operations. That is, unless the user adds another operation, in which
  case those undone operations are destroyed.To implement this, you're
  going to have to do some iterator trickery on the m_operations list in
  the Canvas class. STL and iterators are the only new ILO for this
  workshop.

The Intended Learning Objectives for this workshop are:
- to understand the principles of Object Oriented software design using
  inheritence, polymorphism and data abstraction,
- to extend an existing implementation by learning the existing API,
- to be exposed to some forms of run time type identification.

Richard Southern
21/01/2015
