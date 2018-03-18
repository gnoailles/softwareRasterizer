#pragma once
#include <SDL.h>
#include "Graphics/Texture.h"

using namespace Graphics;

namespace Rendering
{
	class SDL_Manager
	{
	protected:
		SDL_Window * m_window;
		SDL_Renderer* m_renderer;
		Texture* m_activeBuffer;

	public:
		SDL_Manager();
		~SDL_Manager();

		bool Init(int p_w, int p_h);
		void Close();

		Texture* GetBuffer() const;

		void ClearBuffer(const Color& c = Color(0,0,0)) const;
		void UpdateWindow() const;
	};
}
