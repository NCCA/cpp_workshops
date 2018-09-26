#include "canvas.h"
#include "lineoperation.h"
#include "rectangleoperation.h"
#include <iostream>

/**
 * @brief Canvas::Canvas Construct a clean canvas.
 */
Canvas::Canvas()
{
    // Set the last x and y mouse coordinates to a dummy negative value
    m_last_x = m_last_y = -1;

    // Start by assuming the mouse is not clicked
    m_mouseDown = false;
}

/**
 * @brief Canvas::~Canvas Clear away allocated memory
 */
Canvas::~Canvas() {
    OperationList::iterator it;
    for (it = m_operations.begin(); it != m_operations.end(); ++it) {
        delete (*it);
    }
}

/**
 * @brief Canvas::draw This is where all the magic happens.
 * Due to class polymorphism, we don't care about the type of the operation before
 * we call draw.
 * @param r The SDL_Renderer which we'll use to draw this primitive.
 */
void Canvas::draw(SDL_Renderer* r) {
    OperationList::iterator it;

    // Note that we've modified the draw to iterate only to a specified end
    for (it = m_operations.begin(); it != m_operations.end(); ++it) {
    //for (it = m_operations.begin(); it != m_operations.end(); ++it) {
        if ((*it)->apply(r) != EXIT_SUCCESS) exit(0);
    }
}

/**
 * @brief Canvas::mouseDown - record the current mouse position so we can create our primitive
 * @param x
 * @param y
 */
void Canvas::mouseDown(int x, int y) {
    m_last_x = x; m_last_y = y;
}

/**
 * @brief Canvas::mouseMove handle rubber banding behaviour if the mouse has moved
 * while the mouse is held down.
 * @param x
 * @param y
 */
void Canvas::mouseMove(int /*x*/, int /*y*/) {
    // This doesn't do anything at present
}

/**
 * @brief Canvas::mouseUp: create the operation from the mouse input. You'll need to add new primitives here
 * @param x
 * @param y
 */
void Canvas::mouseUp(int x, int y) {
    // This shouldn't be possible, but just in case . . .
    if (m_last_x == -1) return;

    // Here you may want to look into supporting alternative operations . . .
    Operation *op = new LineOperation(m_last_x, m_last_y, x, y);

    // Add our operation to the list
    addOperation(op);
}

/**
 * @brief Canvas::addOperation Handles undo / redo behaviour
 * @param op
 */
void Canvas::addOperation(Operation *op) {
    // This is currently pretty simple: just add the operation to the end of the list
    // It will get more complicated when you implement undo/redo
    m_operations.push_back(op);
}

/**
 * @brief Canvas::handleKey Allows for rudimentary keyboard input
 * @param c The key that was hit
 */
void Canvas::handleKey(char c, int, int) {
    if (c == 'o') {
        cycleOperation();
    } else if (c == 'c') {
        cycleColour();
    } else if (c == 'u') {
        undo();
    } else if (c == 'r') {
        redo();
    }
}

/**
 * @brief Canvas::cycleOperation: change the currently selected operation
 */
void Canvas::cycleOperation() {
    // How are you going to do this?
}

/**
 * @brief Canvas::cycleColours: cycle through the available colours
 */
void Canvas::cycleColour() {
    // How are you going to do this?
}

/**
 * @brief Canvas::undo
 */
void Canvas::undo() {
    // Placeholder
}

/**
 * @brief Canvas::redo
 */
void Canvas::redo() {
    // Placeholder
}
