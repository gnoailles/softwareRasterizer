#include <Graphics/Vertex.h>

using namespace Graphics;

Vertex::Vertex(): m_position(0.0f, 0.0f, 0.0f), m_color(255,255,255)
{
}

Vertex::Vertex(const Vec3& p_vec3, const Color& p_color): m_position(p_vec3), m_color(p_color)
{}

Vertex::Vertex(float x, float y, float z, const Color& p_color): m_position(x,y,z), m_color(p_color)
{}


Vertex::~Vertex()
{}

const Vec3& Vertex::GetPosition() const
{
	return this->m_position;
}

const Color& Vertex::GetColor() const
{
	return this->m_color;
}

void Vertex::SetColor(const Color & p_color)
{
	this->m_color = p_color;
}

void Vertex::SetPosition(const Vec3& p_pos)
{
	this->m_position = p_pos;
}
