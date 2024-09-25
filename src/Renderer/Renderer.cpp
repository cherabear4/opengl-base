#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
    {

    }
}

bool GLPrintError(const char* function, const char* file, int linenum)
{
    while (GLenum error = glGetError())
    {
        std::cout << "ERROR CODE: " << error << " FUNCTION CALLED: " << function << " @ LINE: " << linenum << "IN FILE: " << file << "\n";
        return false;
    }
    return true;
}

void Renderer::clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& vao, const IndexBuffer& ibo, Shader& shader)
{
    clear();
    shader.bind();
    //shader.setUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.0f);
    vao.bind();
    ibo.bind();
    //vbo.bind();
    GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::setColor(Shader& shader, std::string name, float r, float g, float b, float a)
{
    shader.bind();
    shader.setUniform4f(name, r, g, b, a);
}

// Enable or Disable Blending with a boolean
void Renderer::blend(bool value)
{
    switch (value)
    {
    case true:
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        break;
    case false:
        GLCall(glDisable(GL_BLEND));
        break;
    default:
        break;
    }
}