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

		cube.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(-0.5, 0, 2)));
		sphere.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(0.5, 0, 2)));
		Mat4 rotation = Mat4::CreateRotationMatrix(Vec3(2, 2, 2));

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

			//Step 1
//			rasterizer.DrawTriangleBarycenter(triangle, graphics.GetBuffer(),mainScene.GetLights());
			mainScene.GetEntities()[0].ApplyTransformation(rotation);
			mainScene.GetEntities()[1].ApplyTransformation(rotation);
			rasterizer.RenderScene(&mainScene, graphics.GetBuffer());
			graphics.UpdateWindow();
			end = SDL_GetTicks();
			std::cout << 1 / ((end - timer) / 1000.0f) << std::endl;
			timer = end;
		}
		graphics.Close();
	}
	return 0;
}
