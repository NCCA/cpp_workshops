#include "lineoperation.h"

/**
 * @brief LineOperation::LineOperation
 * @param _x1
 * @param _y1
 * @param _x2
 * @param _y2
 */
LineOperation::LineOperation(const int &_x1, const int &_y1, const int &_x2, const int &_y2)
    : Operation(_x1, _y1, _x2, _y2) {
}

/**
 * @brief LineOperation::~LineOperation
 */
LineOperation::~LineOperation() {
}

/**
 * @brief LineOperation::apply
 * @param r
 */
int LineOperation::apply(SDL_Renderer *r) const {
    // Set the draw colour of our renderer
    SDL_SetRenderDrawColor(r, m_colour[0], m_colour[1], m_colour[2], m_colour[3]);

    // Attempt to draw a line onto the renderer
    int result = SDL_RenderDrawLine(r, m_x1, m_y1, m_x2, m_y2);
    if (result != 0) {
        printf( "LineOperation::apply(SDL_Renderer*) - Operation Failed! SDL_Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
