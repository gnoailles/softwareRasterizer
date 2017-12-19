#include <Rendering/SDL_Manager.h>

using namespace Rendering;

SDL_Manager::SDL_Manager() : window{ nullptr }, renderer{ nullptr } 
{}


bool SDL_Manager::Init(int w_w, int w_h)
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS))
	{
		this->window = SDL_CreateWindow(
			"Rasterizer",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w_w,
			w_h,
			SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS);

		if (this->window)
		{
			this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC);

			if (this->renderer)
				return true;
			else
			{
				SDL_Log("SDL Failed to create renderer, SDL_Error: %s\n", SDL_GetError());
				this->Close();
				return false;
			}
		}
		else
		{
			SDL_Log("SDL Failed to create window, SDL_Error: %s\n", SDL_GetError());
			this->Close();
			return false;
		}
	}
	else
	{
		SDL_Log("SDL Failed to initialize, SDL_Error: %s\n", SDL_GetError());
		return false;
	}
}

void SDL_Manager::Close()
{
	if (this->renderer)
	{
		SDL_DestroyRenderer(this->renderer);
		this->renderer = nullptr;
	}

	if (this->window)
	{
		SDL_DestroyWindow(this->window);
		this->window = nullptr;
	}

	SDL_Quit();
}

SDL_Manager::~SDL_Manager()
{
	if (this->window || this->renderer)
		this->Close();
}

void SDL_Manager::ClearScreen()
{
	SDL_SetRenderDrawColor(this->renderer, 20, 20, 20, 255);
	SDL_RenderClear(this->renderer);

}

void SDL_Manager::UpdateWindow()
{
	SDL_RenderPresent(this->renderer);
}

void SDL_Manager::SetColor(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
}

void SDL_Manager::DrawFillRect(int x, int y, int w, int h)
{
	SDL_Rect rect = { x,y,w,h };
	SDL_RenderFillRect(this->renderer, &rect);
}

void SDL_Manager::GetPixelRgb(SDL_Surface *surface, int x, int y, Uint8 &r, Uint8 &g, Uint8 &b)
{
	SDL_LockSurface(surface);
	Uint32 *pixels = (Uint32 *)surface->pixels;

	SDL_GetRGB(pixels[(y * surface->w) + x], surface->format, &r, &g, &b);
	SDL_UnlockSurface(surface);
}
