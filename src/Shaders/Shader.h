#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
private:
    uint32_t m_RendererID;

    // Utility functions to read shader files and check errors
    std::string readFile(const std::string& filepath);
    bool checkCompileErrors(uint32_t shader, const std::string& type);
    bool checkLinkErrors(uint32_t program);

public:
    // Constructor and Destructor
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    // Bind and Unbind the shader program
    void bind() const;
    void unbind() const;

    // Get the renderer ID for use with OpenGL functions
    uint32_t getRendererID() const { return m_RendererID; }

    // Set uniform methods
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniformMat4f(const std::string& name, glm::mat4 matrix);
    void setUniform1i(const std::string& name, int value);


private:
    int getUniformLocation(const std::string& name);
};