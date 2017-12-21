#pragma once
#include <Maths/Vector3.h>
#include <Maths/Vector4.h>

using namespace Maths;

namespace Graphics
{
	class Vertex
	{
	private:
		Vec3 m_position;

	public:
		Vertex();
		Vertex(const Vec3& p_vec3);
		Vertex(float x, float y, float z);
		~Vertex();

		const Vec3& GetPosition() const;
	};
}
