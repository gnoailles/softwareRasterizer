#pragma once
#include <Maths/Vector3.h>
#include <Maths/Vector4.h>
#include "Color.h"

using namespace Maths;

namespace Graphics
{
	class Vertex
	{
	private:
		Vec3 m_position;
		Color m_color;

	public:
		Vertex();
		Vertex(const Vec3& p_vec3, const Color& p_color = Color(255,255,255));
		Vertex(float x, float y, float z, const Color& p_color = Color(255, 255, 255));
		~Vertex();

		const Vec3& GetPosition() const;
		const Color& GetColor() const;

		void SetColor(const Color& p_color);
	};
}
