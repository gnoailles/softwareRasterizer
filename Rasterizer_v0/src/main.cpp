#include <Rendering/SDL_Manager.h>
#include <Maths/Vector3.h>
#include <Rendering/Rasterizer.h>
#include <Rendering/Window.h>

using namespace Rendering;

int main(int argc, char* argv[])
{
	Window mainWindow(800,800);
	mainWindow.Setup();
	mainWindow.Run();
	mainWindow.Close();
	return 0;
}
