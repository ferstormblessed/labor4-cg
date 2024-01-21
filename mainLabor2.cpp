#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <linmath.h>
#define PI 3.1415926535897932384626433832795


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_UP)
    {
        //Beispiel: Behandlung von Pfeiltaste
    }


    if (key == GLFW_KEY_X)
    {
        //Beispiel: Behandlung der Taste
    }

}


int main()
{
    float rotationAngle = 0;
    GLFWwindow* window;
    if (!glfwInit())
        exit(EXIT_FAILURE);
    else
        std::cout << "GLFW init successful!\n";

    window = glfwCreateWindow(1280, 1024, "Hello OpenGL World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);

        //gr. Zeiger
        glLoadIdentity();
        rotationAngle = glfwGetTime() * 360 / 60;
        glRotatef(rotationAngle, 0, 0, -1);
        glLineWidth(3.0);
        glBegin(GL_LINES);
        glColor3f(1.0f, .5f, .5f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.8f, 0.0f);
        glEnd();

        //kleiner Zeiger
        rotationAngle = glfwGetTime() * 360 / 60;
        glLoadIdentity();
        glRotatef(rotationAngle / 60, 0, 0, -1);
        glBegin(GL_LINES);
        glColor3f(0.5f, .5f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.6f, 0.0f);
        glEnd();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwDestroyWindow(window);
    glfwTerminate();

}

