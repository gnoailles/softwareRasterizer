#include <SceneObjects/Scene.h>

using namespace SceneObjects;

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::AddEntity(const Entity& p_entity)
{
	this->m_entities.push_back(p_entity);
}

 std::vector<Entity>& Scene::GetEntities()
{
	return m_entities;
}
