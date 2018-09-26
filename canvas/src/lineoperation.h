#ifndef LINEOPERATION_H
#define LINEOPERATION_H

#include "operation.h"

/**
 * @brief The LineOperation class
 */
class LineOperation : public Operation
{
public:
    /// ctor
    LineOperation(const int &_x1,
                  const int &_y1,
                  const int &_x2,
                  const int &_y2);

    /// dtor
    ~LineOperation();

    /// Apply this operation to the current renderer
    int apply(SDL_Renderer *) const;
};

#endif // LINEOPERATION_H
