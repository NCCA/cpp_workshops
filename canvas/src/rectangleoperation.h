#ifndef RECTANGLEOPERATION_H
#define RECTANGLEOPERATION_H

#include "operation.h"

/**
 * @brief The RectangleOperation class
 */
class RectangleOperation : public Operation
{
public:
    /// ctor
    RectangleOperation(const int &_x1, const int &_y1, const int &_x2, const int &_y2);

    /// dtor
    ~RectangleOperation() {}

    /// Apply this operation
    int apply(SDL_Renderer */*renderer*/) const;

};

#endif // RECTANGLEOPERATION_H
