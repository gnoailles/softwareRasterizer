#include <Graphics/Texture.h>

using namespace Graphics;


Texture::Texture(const unsigned& p_width, const unsigned& p_height) : m_width{p_width}, m_height{p_height}
{
	m_pixels = new Color[m_width * m_height];
}

Texture::~Texture()
{
	delete[] m_pixels;
}

void Texture::SetPixelColor(const unsigned x, const unsigned y, const Color& c) const
{
	if (y >= this->m_height || x >= this->m_width) return;
	this->m_pixels[y * this->m_width + x] = c;
}

void Texture::Clear(const Color& c) const
{
	for (unsigned int i = 0; i < this->m_width * this->m_height; ++i)
	{
		m_pixels[i] = c;
	}
}

Color* Texture::GetPixels() const
{
	return this->m_pixels;
}

unsigned Texture::Width() const
{
	return this->m_width;
}

unsigned Texture::Height() const
{
	return this->m_height;
}