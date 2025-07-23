#pragma once

#define GLEW_STATIC
#include "include/glew.h"
#include "include/glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

struct Vector4
{
    float r;
    float g;
    float b;
    float a;
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

#define RED Vector4{255, 0, 0, 255}
#define GREEN Vector4{0, 255, 0, 255}
#define BLUE Vector4{0, 0, 255, 255}
#define PI 3.1415926

std::string GetShader(std::string &filepath);
unsigned int CompileShader(unsigned int type, std::string &source);
unsigned int CreateShaders(std::string& vertexSource, std::string &fragmentSource);
Vector4 NormalizeRGBValues(Vector4 values);
float NormalizeCoordinate(int coord, int max, char axis);
void AssignColors(Vector4 color);
void DrawCircle(Circle circle, Vector4 color);
void DrawTriangle(Triangle tr, Vector4 color);
void DrawRect(RECT rect, Vector4 color);
