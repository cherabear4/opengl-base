#include "Texture.h"
#include "../vendor/stb/stb_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_filePath(path), m_localBuffer(nullptr), m_Width(0), m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1); // Depends on file format
	m_localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Min mag
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// Clamp modes
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	GLCall(glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	// Free localBuffer
	if (m_localBuffer)
		stbi_image_free(m_localBuffer);
}

Texture::~Texture()
{
	// Delete Texture from GPU
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::bind(uint32_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::unBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
