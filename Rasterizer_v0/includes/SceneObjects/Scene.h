#pragma once
#include "Entity.h"

namespace SceneObjects
{
	class Scene
	{
	private:
		std::vector<Entity> m_entities;
	public:
		Scene();
		~Scene();
	};
}
