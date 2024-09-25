#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererID; // id for every object we create
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
};