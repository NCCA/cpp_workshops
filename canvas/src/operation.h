#ifndef OPERATION_H
#define OPERATION_H

#include <SDL2/SDL.h>

/**
 * @brief The Operation class manages the colour and types of the available operations.
 * An operation is defined as something which mangles the canvas in some way.
 * This probably means you're going to draw a line or rectangle,
 * but one day this could consider non-linear transforms, warping
 * or something equally bonkers.
 */
class Operation
{
public:
    /// Keep track of the supported colours
    typedef enum {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        BLACK = 3
    } Colour;

public:
    /// ctor
    Operation(const int &_x1 = 0,
              const int &_y1 = 0,
              const int &_x2 = 0,
              const int &_y2 = 0);

    virtual ~Operation();

    /// Apply this operation on the specified renderer (note the null definition: this class is PURE VIRTUAL)
    virtual int apply(SDL_Renderer *) const = 0;

    /// Set the colour of this primitive
    void setColour(const Uint8 &r,
                   const Uint8 &g,
                   const Uint8 &b,
                   const Uint8 &a) {
        m_colour[0] = r; m_colour[1] = g; m_colour[2] = b; m_colour[3] = a;
    }

    /// Set the colour using the enum
    void setColour(const Colour &);

    /// Set the end points of this line
    void setEndPoint(const int &_x2, const int &_y2)
       {m_x2 = _x2; m_y2 = _y2;}

    /// Set the start points of this line
    void setStartPoint(const int &_x1, const int &_y1)
       {m_x1 = _x1; m_y1 = _y1;}

protected:
    /// Store the colour of this primitive
    Uint8 m_colour[4];

    /// Start and end coordinates of our operation (mouse input)
    int m_x1, m_y1, m_x2, m_y2;
};


#endif // OPERATION_H
