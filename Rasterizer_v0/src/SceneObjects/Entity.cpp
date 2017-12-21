#include <SceneObjects/Entity.h>

using namespace SceneObjects;

Entity::Entity() : m_mesh{ nullptr }, m_transformation{Mat4::CreateTranslationMatrix(Vec3(0, 0, 0))}
{
}

Entity::Entity(Mesh* p_mesh, const Mat4& p_transform) : m_mesh{p_mesh}, m_transformation{p_transform}
{
}


Entity::~Entity()
{
}

void Entity::ApplyTransformation(const Mat4& p_transform)
{
	this->m_transformation *= p_transform;
}

const Mesh* Entity::GetMesh() const
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
