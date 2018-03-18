#pragma once
#include <Maths/Vector3.h>
#include "Color.h"

using namespace Maths;

namespace Graphics
{
	class Vertex
	{
	private:
		Vec3 m_position;
		Vec3 m_normal;
		Color m_color;

	public:
		Vertex();
		Vertex(const Vertex& p_vertex);
		void operator=(const Vertex& p_vertex);
		Vertex(const Vec3& p_pos, const Vec3& p_norm = Vec3(0.0f,0.0f,0.0f), const Color& p_color = Color(255,255,255));
		Vertex(float x, float y, float z, const Color& p_color = Color(255, 255, 255));
		~Vertex();

		const Vec3& GetPosition() const;
		const Vec3& GetNormal() const;
		const Color& GetColor() const;

		void SetColor(const Color& p_color);
		void SetPosition(const Vec3& p_pos);
		void SetNormal(const Vec3& p_norm); 
		void SetNormal(float x, float y, float z); 
	};
}
