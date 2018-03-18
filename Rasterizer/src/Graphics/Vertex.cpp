#include <Graphics/Vertex.h>

using namespace Graphics;

Vertex::Vertex(): m_position(0.0f, 0.0f, 0.0f),
				  m_normal(0.0f,0.0f,0.0f), 
				  m_color(255,255,255)
{
}

Vertex::Vertex(const Vertex& p_vertex): m_position(p_vertex.m_position),
										m_normal(p_vertex.m_normal.Normalize()),
										m_color(p_vertex.m_color)
{
}

void Vertex::operator=(const Vertex& p_vertex)
{
	this->m_position = p_vertex.m_position;
	this->m_normal = p_vertex.m_normal.Normalize();
	this->m_color = p_vertex.m_color;
}

Vertex::Vertex(const Vec3& p_pos, const Vec3& p_norm, const Color& p_color) : 
				  m_position(p_pos), 
				  m_normal(p_norm.Normalize()),
				  m_color(p_color)
{
}

Vertex::Vertex(const float x, const float y, const float z, const Color& p_color) : 
				  m_position(x, y, z), 
				  m_normal(0, 0, 0), 
				  m_color(p_color)
{
	m_normal.Normalize();
}

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

void Vertex::SetNormal(const float x, const float y, const float z)
{
	this->m_normal = Vec3(x,y,z);
	this->m_normal.Normalize();
}
