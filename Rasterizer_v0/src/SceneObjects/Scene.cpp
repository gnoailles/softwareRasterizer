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

void Scene::AddEntity(const std::shared_ptr<Mesh> p_mesh, const Mat4& p_transform)
{
	this->m_entities.emplace_back(p_mesh,p_transform);
}

void Scene::AddLight(const Light& p_light)
{
	this->m_lights.push_back(p_light);
}

void Scene::AddLight(const Vec3& p_pos, float p_ambient, float p_diffuse, float p_specular)
{
	this->m_lights.emplace_back(p_pos,p_ambient,p_diffuse,p_specular);
}

void Scene::AddLight(float x, float y, float z, float p_ambient, float p_diffuse, float p_specular)
{
	this->m_lights.emplace_back(x, y, z, p_ambient, p_diffuse, p_specular);
}

std::vector<Entity>& Scene::GetEntities()
{
	return m_entities;
}

std::vector<Light>& Scene::GetLights()
{
	return m_lights;
}
