#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	bind();
	vbo.bind();
	const auto& elements = layout.getElements();
	uint32_t offset = 0;
	for (uint32_t i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (void*)offset));
		offset += element.count * VertexBufferElement::size_of(element.type);
	}

}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
	std::cout << "vao bound" << std::endl;
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
	std::cout << "vao unbound" << std::endl;
}
