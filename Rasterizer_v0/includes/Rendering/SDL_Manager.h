#pragma once
#include <SDL.h>

namespace Rendering
{
	class SDL_Manager
	{
	protected:
		SDL_Window * window;
		SDL_Renderer* renderer;
	public:
		SDL_Manager();
		~SDL_Manager();

		bool Init(int w_w, int w_h);
		void Close();

		void GetPixelRgb(SDL_Surface *surface,
			int x,
			int y,
			Uint8 &r,
			Uint8 &g,
			Uint8 &b);

		void SetColor(int r, int g, int b, int a = 255);
		void DrawFillRect(int x, int y, int w = 64, int h = 64);

		void ClearScreen();
		void UpdateWindow();
	};
}