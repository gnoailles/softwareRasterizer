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

void Texture::SetPixelColor(unsigned x, unsigned y, const Color& c)
{
	unsigned line = (y > 0) ? (y - 1) * this->m_width : 0;
	this->m_pixels[line + x] = c;
}
