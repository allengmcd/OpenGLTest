// Includes
#include <stdio.h>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

// Global variables
const GLint global_WIDTH = 800, global_HEIGHT = 600;

// Main Function
int main()
{
    if(!glfwInit())
    {
        printf("GLFW init failed!");
        glfwTerminate();
        return 1;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *MainWindow = glfwCreateWindow(global_WIDTH, global_HEIGHT, "OpenGL Test Project", NULL, NULL);
    if(!MainWindow)
    {
        printf("GLFW window failed!");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(MainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(MainWindow);
    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK)
    {
        printf("GLEW init failed!");
        glfwDestroyWindow(MainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    // Main loop
    while(!glfwWindowShouldClose(MainWindow))
    {
        // Get user input
        glfwPollEvents();

        // Clear Window
        glClearColor(0.4f, 0.2f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(MainWindow);
    }

    return 0;
}