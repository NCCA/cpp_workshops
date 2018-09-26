#include "rectangleoperation.h"
#include <SDL2/SDL.h>

/**
 * @brief RectangleOperation::RectangleOperation
 * @param _x1
 * @param _y1
 * @param _x2
 * @param _y2
 */
RectangleOperation::RectangleOperation(const int &_x1, const int &_y1, const int &_x2, const int &_y2)
    : Operation(_x1, _y1, _x2, _y2)
{
}

/**
 * @brief RectangleOperation::apply
 * @param r
 * @return
 */
int RectangleOperation::apply(SDL_Renderer *r) const {
    // Set the colour of our renderer
    SDL_SetRenderDrawColor(r, m_colour[0], m_colour[1], m_colour[2], m_colour[3]);

    // Note that this initialiser style requires C++11
    SDL_Rect rect = {m_x1, m_y1, m_x2-m_x1, m_y2-m_y1};

    // Attempt to draw a rectangle
    int result = SDL_RenderDrawRect(r, &rect);
    if (result != 0) {
        printf( "RectangleOperation::apply(SDL_Renderer*) - Operation Failed! SDL_Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
