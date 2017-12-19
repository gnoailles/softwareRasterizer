#include <Rendering/SDL_Manager.h>
#include "Maths/Vector3.h"

using namespace Maths;
using namespace Rendering;

int main(int argc, char* argv[])
{
	SDL_Manager graphics;
	Vec3 vec(5.0,2.0,1.0);

	graphics.Init(1024,768);
	graphics.Close();
	return 0;
}
