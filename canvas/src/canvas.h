#ifndef CANVAS_H
#define CANVAS_H

#include <SDL2/SDL.h>
#include <list>
#include "operation.h"


/// A list of operations
typedef std::list<Operation*> OperationList;

/**
 * @brief The Canvas class A basic SDL canvas application.
 */
class Canvas {
public:
    /// Create a canvas
    Canvas();

    /// Clean up the memory
    ~Canvas();

    /// Draw the scene using the current Renderer
    void draw(SDL_Renderer *);

    /// Handle mouse click
    void mouseDown(int, int);

    /// Handle mouse release
    void mouseUp(int, int);

    /// Handle the mouse moving around
    void mouseMove(int, int);

    /// Handle keyboard input
    void handleKey(char, int, int);

private:
    /// Cycle between currently supported operations
    void cycleOperation();

    /// Cycle between current colours
    void cycleColour();

    /// Undo the last operation
    void undo();

    /// Redo the last undone operation
    void redo();

    /// Add a specified operation, taking into accound undo and redo
    void addOperation(Operation *);

    /// A list of operations used
    OperationList m_operations;

    /// The last mouse position, used to create new operations
    int m_last_x, m_last_y;

    /// Keeping track of the mouse state for rubber banding
    bool m_mouseDown;
};

#endif // CANVAS_H
