#include <Rendering/SDL_Manager.h>
#include "Maths/Vector3.h"
#include "Maths/Matrix.h"
#include "Rendering/Rasterizer.h"

using namespace Maths;
using namespace Rendering;

int main(int argc, char* argv[])
{
	SDL_Manager graphics;
	Rasterizer rasterizer;
	Vec3 vec(5.0,2.0,1.0);

	graphics.Init(1024,768);
	rasterizer.DrawLine(4, 6, 1, 0,nullptr);
	system("pause");
	graphics.Close();
	return 0;
}
