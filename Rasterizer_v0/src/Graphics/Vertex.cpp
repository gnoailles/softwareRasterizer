#include <Graphics/Vertex.h>

using namespace Graphics;

Vertex::Vertex(): m_position(0.0f,0.0f,0.0f)
{}

Vertex::Vertex(const Vec3& p_vec3): m_position(p_vec3)
{}

Vertex::Vertex(const Vec4& p_vec4): m_position(p_vec4.x,p_vec4.y,p_vec4.z)
{
}

Vertex::Vertex(float x, float y, float z): m_position(x,y,z)
{}


Vertex::~Vertex()
{}

const Vec3& Vertex::GetPosition() const
{
	return m_position;
}
