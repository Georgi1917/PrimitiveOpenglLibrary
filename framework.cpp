#include "framework.h"

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

Vector4 NormalizeRGBValues(Vector4 values)
{

    return Vector4{values.r / 255, values.g / 255, values.b / 255, values.a / 255};

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

void AssignColors(Vector4 color)
{

    Vector4 normalizeValues = NormalizeRGBValues(color);

    int prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    int uniformLocation = glGetUniformLocation(prog, "u_Color");
    glUniform4f(uniformLocation, normalizeValues.r, normalizeValues.g, normalizeValues.b, normalizeValues.a);

}

void DrawCircle(Circle circle, Vector4 color)
{

    AssignColors(color);

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

void DrawTriangle(Triangle tr, Vector4 color)
{

    AssignColors(color);

    float positions[] = {

        NormalizeCoordinate(tr.A.x, 1280, 'x'), NormalizeCoordinate(tr.A.y, 720, 'y'),
        NormalizeCoordinate(tr.B.x, 1280, 'x'), NormalizeCoordinate(tr.B.y, 720, 'y'),
        NormalizeCoordinate(tr.C.x, 1280, 'x'), NormalizeCoordinate(tr.C.y, 720, 'y')

    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, sizeof(positions) / sizeof(float));

}

void DrawRect(RECT rect, Vector4 color)
{

    AssignColors(color);

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