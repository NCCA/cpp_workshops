// Some crap we need to compile on Windows (not tested)
#ifdef _WIN32
#include <windows.h>
#endif

// Includes needed for SDL and GL
#include <SDL2/SDL.h>
#include <iostream>

// Include the header file for our current scene
#include "canvas.h"

// Change this if you want something different.
#define WINDOW_TITLE "Canvas Application"

// These defines are for the initial window size
#define WIDTH 640
#define HEIGHT 480

// Our scene, which will store all the drawing stuff
Canvas canvas;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The renderer
SDL_Renderer *gRenderer = NULL;

/**
 * @brief initSDL fires up the SDL window and readies it for filling with SDL goodies
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int initSDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    WIDTH,
                                    HEIGHT,
                                    SDL_WINDOW_SHOWN );
        if( gWindow == NULL ) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return EXIT_FAILURE;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
        }
    }

    return EXIT_SUCCESS;
}

/** \mainpage
 * Consider the scenario. In your new job, you're given this code and you've
 * been instructed to implement a number of new features. The application
 * is a basic SDL canvas application - by dragging and dropping the mouse
 * the user can create new primitives, like lines, rectangles and circles.
 * Currently only lines and rectangles are implemented.
 *
 * The new features your boss requires are listed in increasing order of
 * difficulty (as he/she sees it):
 * - Adding the ability to cycle between the available operations by pushing
 *   'o' - this will only affect the next operation drawn. Currently LineOperation
 *   and RectangleOperation are provided (feel free to implement others).
 * - Do the same for the colour of the operation, which can be cycled by
 *   pushing 'c'.
 * - Implement rubber banding - the ability to preview the shape of the primite
 *   before the mouse button is released. This can be implemented by modifying
 *   the existing mouseUp, mouseDown and mouseMove.
 * - Your boss has now decided in her wisdom to require the implementation
 *   of that classic magical operation used in countless applications: the
 *   ability to undo and redo. To recap, this means that the last operation
 *   will be "removed", although it can be restored by performing the redo
 *   operations. That is, unless the user adds another operation, in which
 *   case those undone operations are destroyed.To implement this, you're
 *   going to have to do some iterator trickery on the m_operations list in
 *   the Canvas class.
 *
 * The Intended Learning Objectives for this workshop are:
 * - to understand the principles of Object Oriented software design using
 *   inheritence, polymorphism and data abstraction,
 * - to extend an existing implementation by learning the existing API,
 * - to be exposed to some forms of run time type identification.
 */

/**
 * @brief main The main opengl loop is managed here
 * @param argc Not used
 * @param args Not used
 * @return EXIT_SUCCESS if it went well!
 */
int main( int , char**  ) {
    //Start up SDL and create window
    if( initSDL() == EXIT_FAILURE ) return EXIT_FAILURE;

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Enable text input
    SDL_StartTextInput();

    // Mouse position variables
    int x = 0, y = 0;

    //While application is running
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            // The window has been resized
            if ((e.type == SDL_WINDOWEVENT) &&
                    (e.window.event == SDL_WINDOWEVENT_RESIZED)) {
                SDL_SetWindowSize(gWindow, e.window.data1, e.window.data2);
            }
            //User requests quit
            else if( e.type == SDL_QUIT ) {
                quit = true;
            }
            //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT ) {
                // Only handle the key if it is printable
                char key = e.text.text[0];

                // Handle the user input by the canvas
                SDL_GetMouseState( &x, &y );
                canvas.handleKey(key, x, y );
            }
            // Handle mouse down
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState( &x, &y );
                canvas.mouseDown(x, y);
            } // Handle mouse up
            else if (e.type == SDL_MOUSEBUTTONUP) {
                SDL_GetMouseState( &x, &y );
                canvas.mouseUp(x, y);
            } // Handle mouse moving
            else if (e.type == SDL_MOUSEMOTION) {
                SDL_GetMouseState( &x, &y );
                canvas.mouseMove(x, y);
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        //Render the scene
        canvas.draw(gRenderer);

        //Update screen
        SDL_RenderPresent( gRenderer );
    }

    //Disable text input
    SDL_StopTextInput();

    //Destroy window
    SDL_DestroyWindow( gWindow );

    //Quit SDL subsystems
    SDL_Quit();

    return EXIT_SUCCESS;
}
