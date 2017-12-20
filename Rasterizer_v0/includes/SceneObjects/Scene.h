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


		const std::vector<Entity>& GetEntities() const;
	};
}
