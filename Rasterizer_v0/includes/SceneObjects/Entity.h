#pragma once
#include "Graphics/Mesh.h"
#include "Maths/Matrix.h"

using namespace Graphics;

namespace SceneObjects
{
	class Entity
	{
	private:
		Mesh* m_mesh;
		Mat4 m_transformation;
	public:
		Entity();
		~Entity();
	};
}
