#include "operation.h"

/**
 * @brief Operation::Operation
 */
Operation::Operation(const int &_x1, const int &_y1, const int &_x2, const int &_y2)
    : m_x1(_x1), m_y1(_y1), m_x2(_x2), m_y2(_y2)
{
    setColour(0, 0, 0, 0xFF);
}

Operation::~Operation() {
}

/**
 * @brief Operation::setColour Set the colour based on the enumerated type (useful for colour cycling)
 * @param col
 */
void Operation::setColour(const Colour &col) {
    switch(col) {
    case (RED):
        setColour(0xFF, 0, 0, 0xFF);
        break;
    case (GREEN):
        setColour(0, 0xFF, 0, 0xFF);
        break;
    case (BLUE):
        setColour(0, 0, 0xFF, 0xFF);
        break;
    case (BLACK):
        setColour(0, 0, 0, 0xFF);
        break;
    }
}
