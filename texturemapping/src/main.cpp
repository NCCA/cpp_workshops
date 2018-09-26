// Some crap we need to compile on Windows (not tested)
#ifdef _WIN32
#include <windows.h>
#endif

// Includes needed for SDL and GL
#include <SDL2/SDL.h>

// Include the header file for our current scene
#include "cubescene.h"
#include "objscene.h"
#include "texturedobjscene.h"

// Change this if you want something different.
#define WINDOW_TITLE "Behold! A spinning thingy!"

// These defines are for the initial window size (it can be changed in the resize function)
#define WIDTH 640
#define HEIGHT 480

// Our scene, which will store all the GL stuff
Scene *scene = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

/**
 * @brief initSDL fires up the SDL window and readies it for OpenGL
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int initSDL()
{
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return EXIT_FAILURE;
    }
    else
    {
        //Use OpenGL 3.1 core
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

        //Create window
        gWindow = SDL_CreateWindow( "Behold! A spinning thing!",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    WIDTH,
                                    HEIGHT,
                                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief timerCallback an SDL2 callback function which will trigger whenever the timer has hit the elapsed time.
 * @param interval The elapsed time (not used - scene uses it's own internal clock)
 * @return the elapsed time.
 */
Uint32 timerCallback(Uint32 interval, void *) {
    if (scene != NULL) scene->update();
    return interval;
}

/**
 * @brief main The main opengl loop is managed here
 * @param argc Not used
 * @param args Not used
 * @return EXIT_SUCCESS if it went well!
 */
int main( int argc, char* args[] ) {
    //Start up SDL and create window
    if( initSDL() == EXIT_FAILURE ) return EXIT_FAILURE;

    //Create context
    gContext = SDL_GL_CreateContext( gWindow );
    if( gContext == NULL ) return EXIT_FAILURE;

    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
        fprintf(stderr, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }

    // We should now be ready to use OpenGL
    // This object holds our scene. It needs to be initialised before it can be drawn.
    // Sonic was nicked from http://www.blender-models.com/model-downloads/characters/id/sonic-heroes/
//    scene = new CubeScene();
    scene = new TexturedObjScene("data/teapot.obj", "data/mandril.png");
    //scene = new TexturedObjScene("data/sonic_mesh.obj", "data/sonic_texture.png");

    // Initialise the scene
    scene->init();
    // Need an initial resize to make sure the projection matrix is initialised
    scene->resize(WIDTH, HEIGHT);

    // Use a timer to update our scene. This is the best way to handle updates,
    // as the timer runs in a separate thread and is therefore not affected by the
    // rendering performance.
    SDL_TimerID timerID = SDL_AddTimer(10, /*elapsed time in milliseconds*/
                                     timerCallback, /*callback function*/
                                     (void*) NULL /*parameters (none)*/);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Enable text input
    SDL_StartTextInput();

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
                scene->resize(e.window.data1, e.window.data2);
            }
            //User requests quit
            else if( e.type == SDL_QUIT ) {
                quit = true;
            }
            //Handle keypress with current mouse position
            else if( e.type == SDL_TEXTINPUT ) {
                int x = 0, y = 0;
                SDL_GetMouseState( &x, &y );
                //handleKeys( e.text.text[ 0 ], x, y );
            }
        }

        //Render the scene
        scene->draw();

        //Update screen
        SDL_GL_SwapWindow( gWindow );
    }

    //Disable text input
    SDL_StopTextInput();

    // Disable our timer
    SDL_RemoveTimer(timerID);

    // Delete our scene
    delete scene;

    //Destroy window
    SDL_DestroyWindow( gWindow );

    //Quit SDL subsystems
    SDL_Quit();

    return EXIT_SUCCESS;
}
