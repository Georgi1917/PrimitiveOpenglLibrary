#define GLEW_STATIC
#include "include/glew.h"
#include "include/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#define SPEED 250.0f
#define PI 3.1415926

struct ShaderProgram
{
    std::string vertexSource;
    std::string fragmentSource;
};

struct Vector2
{
    float x;
    float y;
};

struct RECT
{
    float x;
    float y;
    unsigned int width;
    unsigned int height;
};

struct Triangle
{
    Vector2 A;
    Vector2 B;
    Vector2 C;
};

struct Circle
{
    float cx;
    float cy;
    float radius;
};

std::string GetShader(std::string &filepath)
{

    std::ifstream stream(filepath);
    std::string line;

    std::stringstream ss;

    while (std::getline(stream, line))
        ss << line << "\n";

    return ss.str();

}

unsigned int CompileShader(unsigned int type, std::string &source)
{

    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    return id;

}

unsigned int CreateShaders(std::string& vertexSource, std::string &fragmentSource)
{

    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;

}

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

void DrawCircle(Circle circle)
{

    const int segments = 360;
    const int numOfVertices = segments + 2;

    float doublePI = PI * 2.0f;

    float verticesX[numOfVertices];
    float verticesY[numOfVertices];

    verticesX[0] = NormalizeCoordinate(circle.cx, 1280, 'x');
    verticesY[0] = NormalizeCoordinate(circle.cy, 720, 'y');

    for (int i = 1; i < numOfVertices; i++)
    {

        verticesX[i] = NormalizeCoordinate(circle.cx + (circle.radius * cos(i * doublePI / segments)), 1280, 'x');
        verticesY[i] = NormalizeCoordinate(circle.cy + (circle.radius * sin(i * doublePI / segments)), 720, 'y');

    }

    float allVertices[numOfVertices * 2];

    for (int i = 0; i < numOfVertices; i++)
    {

        allVertices[i * 2] = verticesX[i];
        allVertices[(i * 2) + 1] = verticesY[i];

    }

    glBufferData(GL_ARRAY_BUFFER, sizeof(allVertices), allVertices, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLE_FAN, 0, numOfVertices);    

}

void DrawTriangle(Triangle tr)
{

    float positions[] = {

        NormalizeCoordinate(tr.A.x, 1280, 'x'), NormalizeCoordinate(tr.A.y, 720, 'y'),
        NormalizeCoordinate(tr.B.x, 1280, 'x'), NormalizeCoordinate(tr.B.y, 720, 'y'),
        NormalizeCoordinate(tr.C.x, 1280, 'x'), NormalizeCoordinate(tr.C.y, 720, 'y')

    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, sizeof(positions) / sizeof(float));

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

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

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



        DrawTriangle(tr);
        DrawRect(r);
        DrawCircle(c);

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}