#include <Rendering/Rasterizer.h>

using namespace Rendering;

Rendering::Rasterizer::Rasterizer()
{
}


Rasterizer::~Rasterizer()
{
}

void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
}

void Rasterizer::DrawLine(int x1, int y1, int x2, int y2, Texture* pTarget)
{
	Color pixelColor(255, 255, 255);
	uint8_t octant = GetLineOctant(x1, y1, x2, y2);
	SwitchToOctantOne(octant, x1, y1);
	SwitchToOctantOne(octant, x2, y2);
	int dx = x2 - x1;
	int dy = y2 - y1;

	int e = dx;
	dx *= 2;
	dy *= 2;

	while (x1 <= x2)
	{
		SwitchFromOctantOne(octant, x1, y1);
//		pTarget->SetPixelColor(x1, y1, pixelColor);
		std::cout << x1 << " " << y1 << std::endl;
		SwitchToOctantOne(octant, x1, y1);
		++x1;
		e -= dy;
		if (e < 0)
		{
			++y1;
			e += dx;
		}
	}

}

uint8_t Rasterizer::GetLineOctant(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if(dx > 0)
	{
		if(dy > 0)
		{
			if(dx >= dy) return 1;
			else return 2;
		}
		else
		{
			if (dx >= -dy) return 8;
			else return 7;
		}
	}
	else
	{
		if (dy > 0)
		{
			if (-dx >= dy) return 4;
			else return 3;
		}
		else
		{
			if (dx <= dy) return 5;
			else return 6;
		}
	}
}

void Rasterizer::SwitchToOctantOne(const uint8_t octant, int& x, int& y)
{
	switch (octant)
	{
	case 1:		// X,Y
		return;
	case 2:		// Y,X
		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 3:		// Y,-X
		x = -x;

		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 4:		// -X,Y
		x = -x;
		break;
	case 5:		// -X,-Y
		x = -x;
		y = -y;
		break;
	case 6:		// -Y,-X
		x = -x;
		y = -y;

		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 7:		// -Y,X
		y = -y;

		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 8:		// X,-Y
		y = -y;
		break;
	default:
		break;
	}
}

void Rasterizer::SwitchFromOctantOne(const uint8_t octant, int& x, int& y)
{
	switch (octant)
	{
	case 1:		// X,Y
		return;
	case 2:		// Y,X
		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 3:		// -Y,X
		y = -y;

		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 4:		// -X,Y
		x = -x;
		break;
	case 5:		// -X,-Y
		x = -x;
		y = -y;
		break;
	case 6:		// -Y,-X
		x = -x;
		y = -y;

		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 7:		// Y,-X
		x = -x;

		x ^= y;
		y ^= x;
		x ^= y;
		break;
	case 8:		// X,-Y
		y = -y;
		break;
	default:
		break;
	}
}
