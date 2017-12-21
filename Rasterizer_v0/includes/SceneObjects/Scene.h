#pragma once
#include <SceneObjects/Entity.h>

namespace SceneObjects
{
	class Scene
	{
	private:
		std::vector<Entity> m_entities;
	public:
		Scene();
		~Scene();

		void AddEntity(const Entity& p_entity);
		std::vector<Entity>& GetEntities();
	};
}
