#pragma once
#include <memory>
#include <Graphics/Mesh.h>
#include <Maths/Mat4.h>

using namespace Graphics;

namespace SceneObjects
{
	class Entity
	{
	private:
		std::shared_ptr<Mesh> m_mesh;
		Mat4 m_transformation;
	public:
		Entity();
		Entity(const std::shared_ptr<Mesh> p_mesh, const Mat4& p_transform = Mat4::CreateTranslationMatrix(Vec3(0,0,0)));
		~Entity();

		std::shared_ptr<Mesh> GetMesh() const;
		const Mat4& GetTransformation() const;
		void SetTransformation(const Mat4& p_transform);
		void SetColor(const Color& p_color) const;
	};
}
