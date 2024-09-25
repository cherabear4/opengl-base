#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "../Renderer/Renderer.h"

// Constructor: takes file paths for vertex and fragment shaders, compiles and links them
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : m_RendererID(0)
{
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // Compile vertex shader
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, NULL));
    GLCall(glCompileShader(vertexShader));
    if (!checkCompileErrors(vertexShader, "VERTEX")) return;

    // Compile fragment shader
    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL));
    GLCall(glCompileShader(fragmentShader));
    if (!checkCompileErrors(fragmentShader, "FRAGMENT")) return;

    // Link shaders into a program
    m_RendererID = glCreateProgram();
    GLCall(glAttachShader(m_RendererID, vertexShader));
    GLCall(glAttachShader(m_RendererID, fragmentShader));
    GLCall(glLinkProgram(m_RendererID));
    if (!checkLinkErrors(m_RendererID)) return;

    // Cleanup shaders after linking
    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const
{
    std::cout << "shader bound" << std::endl;
    GLCall(glUseProgram(m_RendererID));
}

void Shader::unbind() const
{
    std::cout << "shader unbound" << std::endl;
    GLCall(glUseProgram(0));
}

int Shader::getUniformLocation(const std::string& name)
{
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist or was not used!" << std::endl;
    }
    return location;
}

void Shader::setUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, glm::mat4 matrix)
{
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}

// Utility function to read shader files
std::string Shader::readFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

// Check for shader compilation errors
bool Shader::checkCompileErrors(uint32_t shader, const std::string& type)
{
    int success;
    char infoLog[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        return false;
    }
    return true;
}

// Check for program linking errors
bool Shader::checkLinkErrors(uint32_t program)
{
    int success;
    char infoLog[1024];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << "\n";
        return false;
    }
    return true;
}