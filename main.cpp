#include "framework.h"

#define SPEED 250.0f

void updatePos(double delta, RECT &rect, GLFWwindow* window)
{

    Vector2 velocity = { 0 };

    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
        velocity.x = -1;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
        velocity.x = 1;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
        velocity.y = -1;
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
        velocity.y = 1;

    rect.x += velocity.x * delta * SPEED;
    rect.y += velocity.y * delta * SPEED;

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

    glfwSwapInterval(1);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {

        std::cout << "ERROR!" << "\n";
        return -1;

    }

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);

    double currTime = glfwGetTime();
    double lastTime = currTime;
    double deltaTime;

    Triangle tr = { 0 };
    tr.A = { 200, 450 };
    tr.B = { 400, 450} ;
    tr.C = { 300, 240 };
    RECT r = { 0 };
    r.x = 540;
    r.y = 240;
    r.width = 200;
    r.height = 200;
    Circle c = { 0 };
    c.cx = 940;
    c.cy = 340;
    c.radius = 100;
    RECT testRect = { 0 };
    testRect.x = 60;
    testRect.y = 60;
    testRect.width = 50;
    testRect.height = 50;

    std::string fs = "shaders/fragment.shader";
    std::string vs = "shaders/vertex.shader";

    std::string fsSource = GetShader(fs);
    std::string vsSource = GetShader(vs);

    unsigned int shaderProgram = CreateShaders(vsSource, fsSource);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window))
    {

        currTime = glfwGetTime();
        deltaTime = currTime - lastTime;
        lastTime = currTime;
       
        glClear(GL_COLOR_BUFFER_BIT);

        updatePos(deltaTime, testRect, window);

        // DrawTriangle(tr, RED);
        // DrawRect(r, GREEN);
        // DrawCircle(c, BLUE);
        DrawRect(testRect, RED);

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}