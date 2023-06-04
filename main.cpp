#include <iostream>
#include <SDL.h>
#include <glad/gl.h>
//#include <KHR/khrplatform.h>
//#include <SDL_opengl.h>


//globals:
bool appIsRunning = true;

// Create a window data type
// This pointer will point to the
// window that is allocated from SDL_CreateWindow
SDL_Window* window;
SDL_GLContext context;

int ScreenWidth = 640;
int ScreenHeight = 480;


void GetOpenGLinfo() {
    //funciones de opengl
    std::cout << "hola vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


void InitializeProgram() {
    // Initialize the video subsystem.
    // If it returns less than 1, then an
    // error code will be received.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized: " <<
                  SDL_GetError();
    } else {
        std::cout << "SDL video system is ready to go\n";
    }
    // Before we create our window, specify OpenGL version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); //antialising agregue yo

    // Request a window to be created for our platform
    // The parameters are for the title, x and y position,
    // and the width and height of the window.
    window = SDL_CreateWindow("C++ SDL2 Window",
                              0,
                              00,
                              ScreenWidth,
                              ScreenHeight,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    // OpenGL set up the graphics context
    context = SDL_GL_CreateContext(window);

    // Setup our function pointers (glad)
    //(SDL_GL_GetProcAddress); //esta linea era original cambié por esto:
    int version = gladLoadGL((GLADloadfunc) SDL_GL_GetProcAddress);

    int major = GLAD_VERSION_MAJOR(version);
    int minor = GLAD_VERSION_MINOR(version);
    printf("Loaded OpenGL version %d.%d\n", major, minor);

    GetOpenGLinfo();

    glViewport(0, 0, ScreenWidth, ScreenHeight);
}

//maneja eventos (sdl)
void Input() {
    SDL_Event event;
    // Start our event loop
    while (SDL_PollEvent(&event)) {
        // Handle each specific event
        if (event.type == SDL_QUIT) {
            appIsRunning = false;
        }
        if (event.type == SDL_MOUSEMOTION) {
            std::cout << "mouse has been moved\n";
        }
        if (event.type == SDL_KEYDOWN) {
            std::cout << "a key has been pressed\n";
            if (event.key.keysym.sym == SDLK_0) {
                std::cout << "0 was pressed\n";
            } else {
                std::cout << "0 was not pressed\n";
            }
        }
        // Retrieve the state of all the keys
        // Then we can query the scan code of one or more
        // keys at a time
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_RIGHT]) {
            std::cout << "right arrow key is pressed\n";
        }
    }
}

void PreDraw() {

}

void Draw() {
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void MainLoop() {

    while (appIsRunning) {

        Input();
        PreDraw();
        Draw();

        // update screen
        SDL_GL_SwapWindow(window);
    }

}

void CleanUp() {
    // We destroy our window. We are passing in the pointer
    // that points to the memory allocated by the
    // 'SDL_CreateWindow' function. Remember, this is
    // a 'C-style' API, we don't have destructors.
    SDL_DestroyWindow(window);

    //  SDL_Delay(3000);
    // We safely uninitialize SDL2, that is, we are
    // taking down the subsystems here before we exit
    // our program.
    SDL_Quit();
}


void VertexSpecification() {

}

int main(int argc, char* argv[]) {

    InitializeProgram();

    VertexSpecification();

    // Infinite loop for our application
    MainLoop();

    CleanUp();

    return 0;
}