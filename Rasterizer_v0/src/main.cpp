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
//		Vertex triangle[3] = { Vertex(Vec3(-0.5,-0.5,0,-0.5), Vec3(-0.5,0),Color(255,0,0)), Vertex(Vec3(0.5,-0.5,0),Vec3(0.5,-0.5,0),Color(0,255,0)), Vertex(Vec3(0,0.5,0),Vec3(0,0.5,0),Color(0,0,255)) };

		Entity cube(Mesh::CreateCube());
		Entity sphere(Mesh::CreateSphere(16, 16));

		cube.SetColor(Color(255, 0, 0));
		sphere.SetColor(Color(0, 0, 255));

		float angle = 2.0f;

		mainScene.AddEntity(sphere);
		mainScene.AddEntity(cube);

		mainScene.AddLight(0.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.4f);
		float end = 0;
		float timer = SDL_GetTicks();
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

			mainScene.GetEntities()[0].SetTransformation(Mat4::CreateTransformMatrix(Vec3(1, 0, 2),Vec3(angle, angle, angle), Vec3(1, 1, 1)));
			mainScene.GetEntities()[1].SetTransformation(Mat4::CreateTransformMatrix(Vec3(-1, 0, 2), Vec3(angle, angle, angle), Vec3(1, 1, 1)));

			//Step 1
//			rasterizer.DrawTriangleBarycenter(triangle, graphics.GetBuffer(),mainScene.GetLights());
			rasterizer.RenderScene(&mainScene, graphics.GetBuffer());
			graphics.UpdateWindow();

			end = SDL_GetTicks();
			std::cout << 1 / ((end - timer) / 1000.0f) << std::endl;
			timer = end;

			angle += 2.0f;
		}
		graphics.Close();
	}
	return 0;
}