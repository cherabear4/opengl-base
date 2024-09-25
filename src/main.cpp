#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer/Renderer.h"
#include "VertexUtil/VertexBuffer.h"
#include "VertexUtil/IndexBuffer.h"
#include "VertexUtil/VertexArray.h"
#include "VertexUtil/VertexBufferLayout.h"
#include "shaders/Shader.h"
#include "Texture/Texture.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

int width = 800;
int height = 600;

GLFWwindow* initGLFW(int width, int height, const char* title);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Code written by S. Savage
// Debugging help from chatgpt and gemini
// Comments added by chatgpt

int main() {
    GLFWwindow* window = initGLFW(width, height, "OpenGL Triangle");
    if (window == nullptr) return -1;
    glEnable(GL_DEPTH_TEST);
    Shader shader("Shaders/impl/vertex.glsl", "Shaders/impl/fragment.glsl");

    Texture texture("assets/wall.jpg");

    float vertices[] = {
        // Positions          // Texture Coordinates
        // Base
        -0.5f, 0.0f,  0.5f,  0.0f, 0.0f,  // Base Bottom Left
        -0.5f, 0.0f, -0.5f,  5.0f, 0.0f,  // Base Bottom Right
         0.5f, 0.0f, -0.5f,  0.0f, 0.0f,  // Base Top Right
         0.5f, 0.0f,  0.5f,  5.0f, 0.0f,  // Base Top Left
         // Apex
          0.0f, 1.0f,  0.0f,  2.5f, 5.0f   // Apex
    };

    unsigned int indices[] = {
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    float rotation = 0.0f;
    double prev = glfwGetTime();

    VertexArray vArray;
    VertexBuffer vb(vertices, 5 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    vArray.addBuffer(vb, layout);

    IndexBuffer ib(indices, 12);

    // Projection Matrix
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    // View Matrix
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, -0.5f, -2.0f));

    // Model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 mvp = projectionMatrix * viewMatrix * model;

    Renderer renderer;
    renderer.blend(true);
    texture.bind();

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current = glfwGetTime();

        if (current - prev >= 1.0f / 60.0f) {
            rotation += 0.5f;
            prev = current;
        }

        // Update the model matrix with the new rotation value
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

        // Recalculate the MVP matrix
        glm::mat4 mvp = projectionMatrix * viewMatrix * model;

        shader.bind();
        shader.setUniform1i("u_Texture", 0);

        // Pass the updated MVP matrix to the shader
        shader.setUniformMat4f("u_MVP", mvp);

        vArray.bind();
        ib.bind();

        renderer.draw(vArray, ib, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* initGLFW(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glfwSwapInterval(1); // Enable VSync
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}