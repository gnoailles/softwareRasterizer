#include <Rendering/SDL_Manager.h>
#include <Maths/Vector3.h>
#include <Rendering/Rasterizer.h>

using namespace Maths;
using namespace Rendering;

int main(int argc, char* argv[])
{
	SDL_Manager graphics;
	Scene mainScene;
	if (graphics.Init(1024, 768))
	{
		Rasterizer rasterizer(graphics.GetBuffer()->Width(), graphics.GetBuffer()->Height());
		bool playing = true;
		//Step 0
//		Entity cube(Mesh::CreateCube());
//		Entity sphere(Mesh::CreateSphere(16, 16));
//
//		cube.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(-2.5, 0, 2)));
//		sphere.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(2.5, 0, 2)));
//		mainScene.AddEntity(cube);
//		mainScene.AddEntity(sphere);
//		Mat4 rotation = Mat4::CreateRotationMatrix(Vec3(2, 2, 2));

		//Step 1
		//std::vector<Vertex> triangle = { Vertex(-0.5,-0.5,0,Color(255,0,0)), Vertex(0.5,-0.5,0,Color(0,255,0)), Vertex(0,0.5,0,Color(0,0,255)) };
		//std::vector<Vertex> triangle2 = { Vertex(-0.5,-0.5,-1,Color(255,0,0)), Vertex(0.5,-0.5,-1,Color(255,0,0)), Vertex(0,0.5,-1,Color(255,0,0)) };

		Mesh* cubeMesh = Mesh::CreateCube();
		cubeMesh->SetColor(Color(255, 0, 0));
		Entity cube(cubeMesh);
		
		Mesh* sphereMesh = Mesh::CreateSphere(16, 16);
		sphereMesh->SetColor(Color(0, 0, 255));

		Entity sphere(sphereMesh);

		cube.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(-0.5, 0, 2)));
		sphere.SetTransformation(Mat4::CreateTranslationMatrix(Vec3(0.5, 0, 2)));
		mainScene.AddEntity(cube);
		mainScene.AddEntity(sphere);

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
			//Step 0
//				mainScene.GetEntities()[0].ApplyTransformation(rotation);
//				mainScene.GetEntities()[1].ApplyTransformation(rotation);
//				rasterizer.RenderScene(&mainScene, graphics.GetBuffer());

			//Step 1
			//rasterizer.DrawTriangleBarycenter(triangle, graphics.GetBuffer());
			//rasterizer.DrawTriangleBarycenter(triangle2, graphics.GetBuffer());

			rasterizer.RenderScene(&mainScene, graphics.GetBuffer());
			graphics.UpdateWindow();
		}
		graphics.Close();
	}
	return 0;
}
