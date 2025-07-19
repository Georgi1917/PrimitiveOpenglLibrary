#define GLEW_STATIC
#include "include/glew.h"
#include "include/glfw3.h"

#include <iostream>

struct Vector2
{
    float x;
    float y;
};

struct RECT
{
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

float NormalizeCoordinate(int coord, int max, char axis)
{

    if (axis == 'x') 
    {
        
        return 2.0f * ((float)coord/(float)max) - 1.0f;

    }
    else 
    {
        
        return 1.0f - 2.0f * ((float)coord/(float)max);

    }

}

void DrawRect(RECT rect)
{

    float positions[] = {

        NormalizeCoordinate(rect.x, 1280, 'x'), NormalizeCoordinate(rect.y + rect.height, 720, 'y'),
        NormalizeCoordinate(rect.x, 1280, 'x'), NormalizeCoordinate(rect.y, 720, 'y'),
        NormalizeCoordinate(rect.x + rect.width, 1280, 'x'), NormalizeCoordinate(rect.y + rect.height, 720, 'y'),
        NormalizeCoordinate(rect.x + rect.width, 1280, 'x'), NormalizeCoordinate(rect.y, 720, 'y')

    };

    unsigned int indexBuffer[] = {
        0, 1, 2,
        2, 3, 1
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), indexBuffer, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

int main(void)
{

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    //glfwSetKeyCallback(window, keyCallback);

    if (glewInit() != GLEW_OK)
    {

        std::cout << "ERROR!" << "\n";
        return -1;

    }
  
    RECT rect = { 0 };
    rect.x = 30;
    rect.y = 30;
    rect.width = 40;
    rect.height = 40;
    RECT rect2 = { 0 };
    rect2.x = 350;
    rect2.y = 350;
    rect2.width = 50;
    rect2.height = 50;

    Vector2 velocity = { 0 };

    while (!glfwWindowShouldClose(window))
    {

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            velocity.x = -1;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            velocity.x = 1;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            velocity.y = -1;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            velocity.y = 1;
       
        glClear(GL_COLOR_BUFFER_BIT);
        
        rect.x += velocity.x;
        rect.y += velocity.y;

        DrawRect(rect);

        velocity = { 0 };

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}