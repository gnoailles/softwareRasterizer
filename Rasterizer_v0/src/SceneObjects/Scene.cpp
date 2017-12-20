#include <SceneObjects/Scene.h>

using namespace SceneObjects;

Scene::Scene()
{
}


Scene::~Scene()
{
}

const std::vector<Entity>& Scene::GetEntities() const
{
	return m_entities;
}
