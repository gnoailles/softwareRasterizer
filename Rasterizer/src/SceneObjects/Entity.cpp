#include <SceneObjects/Entity.h>

using namespace SceneObjects;

Entity::Entity() : m_mesh{ nullptr }, m_transformation{Mat4::CreateScaleMatrix(Vec3(1, 1, 1))}
{
}

Entity::Entity(const std::shared_ptr<Mesh> p_mesh, const Mat4& p_transform) : m_mesh{p_mesh}, m_transformation{p_transform}
{
}


Entity::~Entity()
{
}

std::shared_ptr<Mesh> Entity::GetMesh() const
{
	return m_mesh;
}

const Mat4& Entity::GetTransformation() const
{
	return m_transformation;
}

void Entity::SetTransformation(const Mat4& p_transform)
{
	this->m_transformation = p_transform;
}

void Entity::SetColor(const Color& p_color) const
{
	this->m_mesh->SetColor(p_color);
}
