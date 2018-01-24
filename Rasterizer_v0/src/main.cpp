#include <Rendering/SDL_Manager.h>
#include <Maths/Vector3.h>
#include <Rendering/Rasterizer.h>
#include "SceneObjects/Camera.h"

using namespace Maths;
using namespace Rendering;

int main(int argc, char* argv[])
{
	SDL_Manager graphics;
	if (graphics.Init(1280, 720))
	{
		Scene mainScene;
		const Rasterizer rasterizer(graphics.GetBuffer()->Width(), graphics.GetBuffer()->Height());

		Mat4 camera = Mat4::CreateTranslationMatrix(Vec3(0.0,0.0,0));
		camera = camera.Inverse();

		const Mat4& proj = Rasterizer::CreatePerspectiveProjectionMatrix(graphics.GetBuffer()->Width(),
																		graphics.GetBuffer()->Height(),
																		0.1, 100, 90);
		bool playing = true;

		//Step 1
		//		Vertex triangle[3] = { Vertex(Vec3(-0.5,-0.5,0,-0.5), Vec3(-0.5,0),Color(255,0,0)), Vertex(Vec3(0.5,-0.5,0),Vec3(0.5,-0.5,0),Color(0,255,0)), Vertex(Vec3(0,0.5,0),Vec3(0,0.5,0),Color(0,0,255)) };

		Entity sphere(Mesh::CreateSphere(16,16));
		Entity cube(Mesh::CreateCube());

		cube.SetColor(Color(255, 0, 0));
		sphere.SetColor(Color(0, 0, 255));

		sphere.SetTransformation(Mat4::CreateTransformMatrix(Vec3(-1, 0, -5), Vec3(0, 0, 0), Vec3(1, 1, 1)));

		float angle = 2.0f;

		mainScene.AddEntity(sphere);
		mainScene.AddEntity(cube);

		mainScene.AddLight(0.0f, 0.0f, 0.0f, 0.2f, 0.4f, 0.4f);
		float end = 0;
		float timer = SDL_GetTicks();

		//		proj.Transpose();
		while (playing)
		{
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				{
					playing = false;
					break;
				}
			}
			graphics.ClearBuffer();

			mainScene.GetEntities()[1].SetTransformation(Mat4::CreateTransformMatrix(Vec3(1, 0, -5), Vec3(angle, angle, angle), Vec3(1, 1, 1)));
			mainScene.GetEntities()[0].SetTransformation(Mat4::CreateTransformMatrix(Vec3(-1, 0, -5), Vec3(-angle, angle, -angle), Vec3(1, 1, 1)));

			//Step 1
			//rasterizer.DrawTriangleBarycenter(triangle, graphics.GetBuffer(),mainScene.GetLights());
			rasterizer.RenderScene(&mainScene, graphics.GetBuffer(), proj, camera);
			graphics.UpdateWindow();

			end = SDL_GetTicks();
			//std::cout << 1 / ((end - timer) / 1000.0f) << std::endl;
			timer = end;

			angle += 2.0f;
			if (angle > 360)
				angle = fmod(angle, 360.0f);
		}
		graphics.Close();
	}
	return 0;
}
