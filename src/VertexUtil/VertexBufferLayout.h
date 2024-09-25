#pragma once
#include "../Renderer/Renderer.h"
#include <vector>

struct VertexBufferElement
{
	uint32_t type;
	uint32_t count;
	bool normalized;

    // Returns the size of OpenGL types in bytes
    static int size_of(GLenum type)
    {
        switch (type)
        {
        case GL_BYTE: return 1;
        case GL_UNSIGNED_BYTE: return 1;
        case GL_SHORT: return 2;
        case GL_UNSIGNED_SHORT: return 2;
        case GL_INT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_FLOAT: return 4;
        default: return 0; // or throw an error
        }
    }
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride;
public:
    VertexBufferLayout()
        : m_Stride(0)
    {}

    // Template system to create a custom push function for our layout
    template<typename T>
    void push(int count)
    {
        static_assert(sizeof(T) == 0, "Unsupported type for VertexBufferLayout::push.");
    }

    template<>
    void push<float>(int count)
    {
        m_Elements.push_back({ GL_FLOAT, (uint32_t)count, false });
        m_Stride += VertexBufferElement::size_of(GL_FLOAT) * count;
    }

    template<>
    void push<uint32_t>(int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, (uint32_t)count, false });
        m_Stride += VertexBufferElement::size_of(GL_UNSIGNED_INT) * count;
    }

    template<>
    void push<unsigned char>(int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, (uint32_t)count, true });
        m_Stride += VertexBufferElement::size_of(GL_UNSIGNED_BYTE) * count;
    }

    const std::vector<VertexBufferElement> getElements() const& { return m_Elements; }
    uint32_t getStride() const { return m_Stride; }
};