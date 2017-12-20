#include <Rendering/Rasterizer.h>

using namespace Rendering;

Rasterizer::Rasterizer()
{
}


Rasterizer::~Rasterizer()
{
}

void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
	pTarget->Clear(Color(0, 0, 0));
	std::vector<Entity> entities = pScene->GetEntities();
	for (Entity entity : entities)
	{
		Mesh* mesh = entity.GetMesh();
		for (int i = 0; i < mesh->GetTriangleCount(); ++i)
		{
			Vertex* triangle = mesh->GetTriangleVertices(i);
			Vertex transformedTriangle[3] = { 
				TransformPos(triangle[0].GetPosition(), entity.GetTransformation()),
				TransformPos(triangle[1].GetPosition(), entity.GetTransformation()),
				TransformPos(triangle[3].GetPosition(), entity.GetTransformation()) };

			DrawTriangle(transformedTriangle, pTarget);
		}
	}
}

void Rasterizer::DrawTriangle(const Vertex*, Texture* pTarget)
{
	//TODO Implement Triangle split then Bresenham drawing
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

Vec4 Rasterizer::TransformPos(const Vertex& v, Mat4 transform)
{
	return transform * Vec4(v.GetPosition());
}

void Rasterizer::WorldToScreenCoord(int worldWidth, int worldHeight, 
									int screenWidth, int screenHeight, 
									const Vec3& pos, int& targetX, int& targetY)
{
	targetX = ((pos.x / worldWidth) + 1) * 0.5f * screenWidth;
	targetY = ((pos.y / worldHeight) + 1) * 0.5f * screenHeight;
}

uint8_t Rasterizer::GetLineOctant(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx > 0)
	{
		if (dy > 0)
		{
			if (dx >= dy) return 1;
			return 2;
		}
		if (dx >= -dy) return 8;
		return 7;
	}
	if (dy > 0)
	{
		if (-dx >= dy) return 4;
		return 3;
	}
	if (dx <= dy) return 5;
	return 6;
}

void Rasterizer::SwitchToOctantOne(const uint8_t octant, int& x, int& y)
{
	switch (octant)
	{
	case 1:		// X,Y
		return;
	case 2:		// Y,X
		std::swap(x, y);
		break;
	case 3:		// Y,-X
		x = -x;
		std::swap(x, y);

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
		std::swap(x, y);

		break;
	case 7:		// -Y,X
		y = -y;
		std::swap(x, y);

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
		std::swap(x, y);
		break;
	case 3:		// -Y,X
		y = -y;
		std::swap(x, y);
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
		std::swap(x, y);
		break;
	case 7:		// Y,-X
		x = -x;
		std::swap(x, y);
		break;
	case 8:		// X,-Y
		y = -y;
		break;
	default:
		break;
	}
}
