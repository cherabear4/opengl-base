#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../VertexUtil/VertexArray.h"
#include "../VertexUtil/IndexBuffer.h"
#include "../Shaders/Shader.h"
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak()
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLPrintError(#x, __FILE__, __LINE__))

void GLClearError();
bool GLPrintError(const char* function, const char* file, int linenum);

class Renderer
{
public:
    void clear();
    void draw(const VertexArray& vao, /*VertexBuffer& vbo*/ const IndexBuffer& ibo, Shader& shader);
    void setColor(Shader& shader, std::string name, float r, float g, float b, float a);
    void blend(bool value);
};