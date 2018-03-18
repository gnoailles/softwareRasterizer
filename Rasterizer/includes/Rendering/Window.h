#pragma once
#include <Rendering/SDL_Manager.h>
#include <SceneObjects/Scene.h>
#include <Rendering/Rasterizer.h>

using namespace SceneObjects;

namespace Rendering
{
	class Window
	{
	private:
		SDL_Manager m_graphics;
		Scene m_mainScene;
		Rasterizer* m_rasterizer;
		uint8_t m_rasterizerFlags;
		bool m_isRunning;
	public:
		Window(const unsigned& p_width, const unsigned& p_height);
		~Window();
		void Setup();
		void Run();
		void Close();

		void HandleInputs();

		void Resize(const unsigned& p_width, const unsigned& p_height) const;
	};
}
