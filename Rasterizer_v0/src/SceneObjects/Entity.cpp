#include <SceneObjects/Entity.h>

using namespace SceneObjects;

Entity::Entity()
{
}


Entity::~Entity()
{
}

Mesh* const& Entity::GetMesh() const
{
	return m_mesh;
}

const Mat4& Entity::GetTransformation() const
{
	return m_transformation;
}
