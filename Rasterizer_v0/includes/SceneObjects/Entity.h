#pragma once
#include <Graphics/Mesh.h>
#include <Maths/Mat4.h>

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
		Entity(Mesh* p_mesh, const Mat4& p_transform = Mat4::CreateTranslationMatrix(Vec3(0,0,0)));
		~Entity();

		void ApplyTransformation(const Mat4& p_transform);
		const Mesh* GetMesh() const;
		const Mat4& GetTransformation() const;
		void SetTransformation(const Mat4& p_transform);
	};
}
