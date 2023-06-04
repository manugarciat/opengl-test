#include <iostream>
#include <SDL.h>
#include <glad/gl.h>
#include <vector>
//#include <KHR/khrplatform.h>
//#include <SDL_opengl.h>


//globals:
bool appIsRunning = true;

// Create a window data type
// This pointer will point to the
// window that is allocated from SDL_CreateWindow
SDL_Window *window;
SDL_GLContext context;

int ScreenWidth = 640;
int ScreenHeight = 480;

//VAO
GLuint gVertexArrayObject = 0;

//VBO
GLuint gVertexBufferObject = 0;

//program object para nuestros shaders
GLuint gGraphicsPipelineShaderProgram = 0;

//vertex shader executes once per vertex and es el responsable de la position del vertex
const std::string gVertexShaderSource =
        " #version 410 core\n"
        "\n"
        "in vec4 Position;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = vec4(Position.x, Position.y, Position.z, Position.w);\n"
        "}";

//fragment shader executes once per fragment ( pixel) y determina en parte el color
const std::string  gFragmentShaderSource =
        "#version 410 core\n"
        "\n"
        "out vec4 color;\n"
        "\n"
        "void main() {\n"
        "    color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
        "}";

void GetOpenGLinfo() {
    //funciones de opengl
    std::cout << "hola vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


void VertexSpecification() {

    //lives on the cpu:
    const std::vector<GLfloat> vertexPosition{
            // x     y     z
            -0.8f, -0.8f, 0.0f,
            0.8f, -0.8f, 0.0f,
            0.0f, 0.8f, 0.0f};


    //empieza el seteo en el GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject); //bind: seleccionar VAO para usar

    //generate VBO (vertex buffer object)
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexArrayObject);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertexPosition.size() * sizeof(GLfloat)),
                 vertexPosition.data(), //return de pointer of the raw array, si tengo array paso directo el array.
                 GL_STATIC_DRAW); // por ahora solo vamos a dibujar el triangulo

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, //el buffer tiene x,y,z (3 elementos)
                          GL_FLOAT, GL_FALSE, 0, nullptr);

    //clean
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);


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
        const Uint8 *state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_RIGHT]) {
            std::cout << "right arrow key is pressed\n";
        }
    }
}

void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, ScreenWidth, ScreenHeight);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw() {
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    glDrawArrays(GL_TRIANGLES, 0, 3);

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

GLuint CompileShader(GLuint type, const std::string &source) {
    GLuint shaderObject;
    if (type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    } else shaderObject = 0;

    const char* src = source.c_str(); //pasarlo a string de C
    glShaderSource(shaderObject, 1, &src, nullptr );
    glCompileShader(shaderObject);
    return shaderObject;
}

GLuint CreateShaderProgram(const std::string &vertexshadersource, const std::string &fragmentshadersource) {

    GLuint programObject = glCreateProgram();
    GLuint myVertexshader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexshader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    //validate our program
    glValidateProgram(programObject);

    //gldetachshader y gldeleteshader para limpiar?

    return programObject;
}

void CreategraphicsPipeline() {
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}


int main(int argc, char *argv[]) {

    InitializeProgram();

    VertexSpecification();

    CreategraphicsPipeline();

    // Infinite loop for our application
    MainLoop();

    CleanUp();

    return 0;
}