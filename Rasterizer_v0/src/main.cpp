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
//		cube.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(-2.5, 0, 2)));
		std::vector<Vertex> triangle = { Vec3(-2.5,-4), Vec3(2.5,1), Vec3(-2,4)};
		mainScene.AddEntity(cube);
		Mat4 rotation = Mat4::CreateRotationMatrix(Vec3(2, 2, 2));
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
			mainScene.GetEntities()[0].ApplyTransformation(rotation);
			rasterizer.RenderScene(&mainScene, graphics.GetBuffer());
//			rasterizer.DrawTriangle(triangle, graphics.GetBuffer());
			graphics.UpdateWindow();
		}
		graphics.Close();
	}
	return 0;
}
