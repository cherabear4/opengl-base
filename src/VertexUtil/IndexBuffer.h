#pragma once


class IndexBuffer
{
private:
	unsigned int m_RendererID; // id for every object we create
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	unsigned int getCount() const { return m_Count; };
};