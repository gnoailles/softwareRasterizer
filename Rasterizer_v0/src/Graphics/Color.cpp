#include <Graphics/Color.h>

using namespace Graphics;

Color::Color(): r(0), g(0), b(0), a(255)
{
}

Color::Color(unsigned char p_r, unsigned char p_g, unsigned char p_b, unsigned char p_a): r(p_r), g(p_g), b(p_b), a(p_a)
{
}

Color::Color(const Color& c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
	this->a = c.a;
}

Color::~Color()
{
}
