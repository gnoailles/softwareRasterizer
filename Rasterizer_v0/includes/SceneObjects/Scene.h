#pragma once
#include <SceneObjects/Entity.h>
#include <SceneObjects/Light.h>
#include <SceneObjects/Camera.h>

namespace SceneObjects
{
	class Scene
	{
	private:
		std::vector<Entity> m_entities;
		std::vector<Light> m_lights;
		std::vector<Camera> m_cameras;
	public:
		Scene();
		~Scene();

		void AddEntity(const Entity& p_entity);
		void AddEntity(const std::shared_ptr<Mesh> p_mesh, const Mat4& p_transform = Mat4::CreateTranslationMatrix(Vec3(0, 0, 0)));

		void AddLight(const Light& p_light);
		void AddLight(const Vec3& p_pos, float p_ambient, float p_diffuse, float p_specular);
		void AddLight(float x, float y, float z, float p_ambient, float p_diffuse, float p_specular);

		void AddCamera(const int& p_width, const int& p_height, const float& p_near,
			const float& p_far, const float& p_fov,
			const Mat4& p_modelView = Mat4::CreateTranslationMatrix(Vec3(0, 0, 0)));

		std::vector<Entity>& GetEntities();
		std::vector<Light>& GetLights();
		std::vector<Camera>& GetCameras();
		Camera& GetMainCamera();
	};
}
