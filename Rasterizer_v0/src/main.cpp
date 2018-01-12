#include <Rendering/SDL_Manager.h>
#include <Maths/Vector3.h>
#include <Rendering/Rasterizer.h>

using namespace Maths;
using namespace Rendering;

int main(int argc, char* argv[])
{
	SDL_Manager graphics;
	if (graphics.Init(800, 800))
	{
		Scene mainScene;
		Rasterizer rasterizer(graphics.GetBuffer()->Width(), graphics.GetBuffer()->Height());
		bool playing = true;

		//Step 1
//		std::vector<Vertex> triangle = { Vertex(-0.5,-0.5,0,Color(255,0,0)), Vertex(0.5,-0.5,0,Color(0,255,0)), Vertex(0,0.5,0,Color(0,0,255)) };

		Entity cube(Mesh::CreateCube());
		Entity sphere(Mesh::CreateSphere(16, 16));

		cube.SetColor(Color(255, 0, 0));
		sphere.SetColor(Color(0, 0, 255));

		cube.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(-0.5, 0, 2)));
		sphere.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(0.5, 0, 2)));

		mainScene.AddEntity(sphere);
		mainScene.AddEntity(cube);

		while (playing)
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				{
					playing = false;
					break;
				}
			}

			graphics.ClearBuffer();

			//Step 1
			//rasterizer.DrawTriangleBarycenter(triangle, graphics.GetBuffer());

			rasterizer.RenderScene(&mainScene, graphics.GetBuffer());
			graphics.UpdateWindow();
		}
		graphics.Close();
	}
	return 0;
}
