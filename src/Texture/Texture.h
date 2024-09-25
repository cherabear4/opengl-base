#pragma once
#include "../Renderer/Renderer.h"

class Texture
{
private:
	uint32_t m_RendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture(const std::string& path);
	~Texture();
	void bind(uint32_t slot = 0) const;
	void unBind();
	
	int getWidth() const { return m_Width; }
	int getHeight() const { return m_Height; }
};