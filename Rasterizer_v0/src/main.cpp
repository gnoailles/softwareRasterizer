#include <Rendering/SDL_Manager.h>
#include <Maths/Vector3.h>
#include <Rendering/Rasterizer.h>

using namespace Maths;
using namespace Rendering;

int main(int argc, char* argv[])
{
	SDL_Manager graphics;
	Rasterizer rasterizer;
	Scene mainScene;
	if (graphics.Init(1024, 768))
	{
		bool playing = true;
		Entity cube(Mesh::CreateCube());
		cube.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(-2.5, 0, 2)));
		mainScene.AddEntity(cube);
		while (playing)
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT)
				{
					playing = false;
					break;
				}
			}

			graphics.ClearBuffer();
			mainScene.GetEntities()[0].ApplyTransformation(Mat4::CreateTransformMatrix(Vec3(2, 2, 2), Vec3(-0, 0, 0), Vec3(1, 1, 1)));
			rasterizer.RenderScene(&mainScene, graphics.GetBuffer());
			graphics.UpdateWindow();
		}
		graphics.Close();
	}
	return 0;
}
