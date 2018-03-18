#include <Rendering/Window.h>

using namespace Rendering;

Window::Window(const unsigned& p_width, const unsigned& p_height): m_rasterizerFlags(0), m_isRunning(true)
{
	if (m_graphics.Init(p_width, p_height))
	{
		this->m_rasterizer = new Rasterizer(m_graphics.GetBuffer()->Width(), m_graphics.GetBuffer()->Height());
	}
}


Window::~Window()
{
	m_graphics.Close();
	delete this->m_rasterizer;
}

void Window::Setup()
{

	this->m_mainScene.AddCamera(m_graphics.GetBuffer()->Width(), m_graphics.GetBuffer()->Height(), 0.1f, 8, 70);

	Entity triangle(Mesh::CreateTriangle(Vertex(Vec3(-0.5f, -0.5f, 0.f), Vec3(-0.5f, 0.5f, 0.f), Color(255, 0, 0)),
		Vertex(Vec3(0.5f, -0.5f, 0.f), Vec3(0.5f, -0.5f, 0.f), Color(0, 255, 0)),
		Vertex(Vec3(0.f, 0.5f, 0.f), Vec3(0.f, 0.5f, 0.f), Color(0, 0, 255))));
	Entity sphere(Mesh::CreateSphere(32, 32));
	Entity cube(Mesh::CreateCube());

	cube.SetColor(Color(255, 0, 0));
	sphere.SetColor(Color(0, 0, 255));

	triangle.SetTransformation(Mat4::CreateTransformMatrix(Vec3(0, 0, -2), Vec3(0, 0, 0), Vec3(1, 1, 1)));
	cube.SetTransformation(Mat4::CreateTransformMatrix(Vec3(-1, 0, -3), Vec3(0, 0, 0), Vec3(1, 1, 1)));
	sphere.SetTransformation(Mat4::CreateTransformMatrix(Vec3(1, 0, -3), Vec3(0, 0, 0), Vec3(1, 1, 1)));

	m_mainScene.AddEntity(triangle);
	m_mainScene.AddEntity(cube);
	m_mainScene.AddEntity(sphere);

	m_mainScene.AddLight(0.0f, 0.0f, -4.0f, 0.2f, 0.4f, 0.4f);
}

void Window::Run()
{
	float angle = 2.0f;
	while (this->m_isRunning)
	{
		this->HandleInputs();

		this->m_graphics.ClearBuffer();

		//mainScene.GetEntities()[0].SetTransformation(Mat4::CreateTransformMatrix(Vec3(0, 0, 5), Vec3(0, 0, 0), Vec3(1, 1, 1)));
		this->m_mainScene.GetEntities()[1].SetTransformation(Mat4::CreateTransformMatrix(Vec3(-1, 0, -3), Vec3(angle, angle, angle), Vec3(1, 1, 1)));

		//Step 1
		//rasterizer.DrawTriangleBarycenter(triangle, graphics.GetBuffer(),mainScene.GetLights());
		this->m_rasterizer->RenderScene(&this->m_mainScene, this->m_graphics.GetBuffer(), this->m_mainScene.GetMainCamera(), this->m_rasterizerFlags);
		this->m_graphics.UpdateWindow();

//		end = SDL_GetTicks();
		//std::cout << 1 / ((end - timer) / 1000.0f) << std::endl;
//		timer = end;

		angle += 2.0f;
		if (angle > 360)
			angle = fmod(angle, 360.0f);
	}
}

void Window::Close()
{
	this->m_isRunning = false;
}

void Window::HandleInputs()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
			case SDL_QUIT:
			{
				this->Close();
				break;
			}
			case SDL_KEYDOWN:
			{
				Camera & camera = this->m_mainScene.GetMainCamera();
				const float step = 0.5f;
				switch (e.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					{
						this->Close();
						break;
					}
					case SDLK_F1:
					{
						this->m_rasterizerFlags ^= RAST_WIREFRAME;
						break;
					}					
					case SDLK_F2:
					{
						this->m_rasterizerFlags ^= RAST_PHONG;
						this->m_rasterizerFlags &= ~RAST_BLINN_PHONG;
						this->m_rasterizerFlags &= ~RAST_PIXEL_BLINN_PHONG;

						break;
					}				
					case SDLK_F3:
					{
						this->m_rasterizerFlags ^= RAST_BLINN_PHONG;
						this->m_rasterizerFlags &= ~RAST_PIXEL_BLINN_PHONG;
						this->m_rasterizerFlags &= ~RAST_PHONG;
						break;
					}					
					case SDLK_F4:
					{
						this->m_rasterizerFlags ^= RAST_PIXEL_BLINN_PHONG;
						this->m_rasterizerFlags &= ~RAST_BLINN_PHONG;
						this->m_rasterizerFlags &= ~RAST_PHONG;
						break;
					}			
					case SDLK_o:
					{
						this->m_rasterizerFlags |= RAST_ORTHO;
						camera.UseOrthographic();
						break;
					}		
					case SDLK_p:
					{
						this->m_rasterizerFlags &= ~RAST_ORTHO;
						camera.UsePerspective();
						break;
					}
					case SDLK_LEFT:
					{
						camera.SetPosition(Vec3(-step, 0.f, 0.f) + camera.GetPosition());
						break;
					}
					case SDLK_RIGHT:
					{
						camera.SetPosition(Vec3(step, 0.f, 0.f) + camera.GetPosition());
						break;
					}
					case SDLK_UP:
					{
						camera.SetPosition(Vec3(0.f, 0.f, -step) + camera.GetPosition());
						break;
					}
					case SDLK_DOWN:
					{
						camera.SetPosition(Vec3(0.f, 0.f, step) + camera.GetPosition());
						break;
					}
				default: ;
				}
				break;
			}
			case SDL_WINDOWEVENT:
			{
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
					this->Resize((unsigned)e.window.data1, (unsigned)e.window.data2);
				break;
			}
		default: ;
		}
	}
}

void Window::Resize(const unsigned& p_width, const unsigned& p_height) const
{
	const float targetRatio = this->m_graphics.GetBuffer()->Width() / (float)this->m_graphics.GetBuffer()->Height();

	unsigned viewWidth = p_width;
	unsigned viewHeight = static_cast<unsigned>(viewWidth / targetRatio + 0.5f);

	if (viewHeight > p_height)
	{
		viewHeight = p_height;
		viewWidth = static_cast<unsigned>(viewHeight * targetRatio + 0.5f);
	}

	this->m_rasterizer->SetViewport((p_width - viewWidth) / 2, (p_height - viewHeight) / 2, viewWidth, viewHeight);
}
