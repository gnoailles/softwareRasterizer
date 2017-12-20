#include <Graphics/Vertex.h>

using namespace Graphics;

Vertex::Vertex(): m_position(0.0f,0.0f,0.0f)
{}

Vertex::Vertex(const Vec3& p_vec3): m_position(p_vec3)
{}

Vertex::Vertex(float x, float y, float z): m_position(x,y,z)
{}


Vertex::~Vertex()
{}

const Vec3& Vertex::GetPosition() const
{
	return m_position;
}
