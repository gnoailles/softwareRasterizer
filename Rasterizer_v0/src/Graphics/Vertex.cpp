#include <Graphics/Vertex.h>

using namespace Graphics;

Vertex::Vertex(): m_position(0.0f, 0.0f, 0.0f),
				  m_normal(0.0f,0.0f,0.0f), 
				  m_color(255,255,255)
{
}

Vertex::Vertex(const Vec3& p_pos, const Vec3& p_norm, const Color& p_color) : 
				  m_position(p_pos), 
				  m_normal(p_norm.Normalize()),
				  m_color(p_color)
{
}

Vertex::Vertex(float x, float y, float z, const Color& p_color) : 
				  m_position(x, y, z), 
				  m_normal(0, 0, 0), 
				  m_color(p_color)
{}

Vertex::~Vertex()
{}

const Vec3& Vertex::GetPosition() const
{
	return this->m_position;
}

const Vec3& Vertex::GetNormal() const
{
	return this->m_normal;
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

void Vertex::SetNormal(const Vec3& p_norm)
{
	this->m_normal = p_norm;
}

void Vertex::SetNormal(float x, float y, float z)
{
	this->m_normal = Vec3(x,y,z);
}
