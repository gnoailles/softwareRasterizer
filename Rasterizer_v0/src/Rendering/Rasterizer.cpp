#include <Rendering/Rasterizer.h>
#include <algorithm>

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
		const Mesh* mesh = entity.GetMesh();
		for (unsigned i = 0; i < mesh->GetTriangleCount(); ++i)
		{
			const Vertex* triangle = mesh->GetTriangleVertices(i);
			const std::vector<Vertex> transformedTriangle = { 
				TransformPos(triangle[0].GetPosition(), entity.GetTransformation()).ToVec3(),
				TransformPos(triangle[1].GetPosition(), entity.GetTransformation()).ToVec3(),
				TransformPos(triangle[2].GetPosition(), entity.GetTransformation()).ToVec3() };

			delete[] triangle;
			triangle = nullptr;
			DrawTriangle(transformedTriangle, pTarget);
		}
	}
}

void Rasterizer::DrawTriangle(std::vector<Vertex> p_triangle, Texture* p_target)
{
	if (p_triangle.size() != 3)
		return;

	SortVerticesBy(p_triangle);
	int v1x, v1y;
	int v2x, v2y;
	int v3x, v3y;

	WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[0].GetPosition(), v1x, v1y);
	WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[1].GetPosition(), v2x, v2y);
	WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[2].GetPosition(), v3x, v3y);
	
	if (v2y == v3y)
	{
		DrawBottomFlatTriangle(std::vector<Vertex> {Vec3(v1x,v1y),Vec3(v2x,v2y),Vec3(v3x,v3y)}, p_target);
	}
	else if (v1y == v2y)
	{
		DrawTopFlatTriangle(std::vector<Vertex> {Vec3(v1x, v1y), Vec3(v2x, v2y), Vec3(v3x, v3y)}, p_target);
	}
	else
	{
		const Vec3 v4 = Vec3( (int)(v1x +  (float)(v2y - v1y) / (float)(v3y - v1y) * (v3x - v1x)) , v2y);

		const std::vector<Vertex> bottomFlatTriangle = {Vec3(v1x,v1y), Vec3(v2x,v2y), v4};
		const std::vector<Vertex> topFlatTriangle = { Vec3(v2x,v2y), v4, Vec3(v3x,v3y) };
		DrawBottomFlatTriangle(bottomFlatTriangle, p_target);
		DrawTopFlatTriangle(topFlatTriangle, p_target);
	}

}

void Rasterizer::DrawBottomFlatTriangle(const std::vector<Vertex>& p_triangle, Texture* p_target)
{
	float slope1 = (p_triangle[1].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[1].GetPosition().y - p_triangle[0].GetPosition().y);
	float slope2 = (p_triangle[2].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[2].GetPosition().y - p_triangle[0].GetPosition().y);

	float curx1 = p_triangle[0].GetPosition().x, curx2 = p_triangle[0].GetPosition().x;

	for (int scanlineY = p_triangle[0].GetPosition().y; scanlineY <= p_triangle[1].GetPosition().y; ++scanlineY)
	{
		DrawHorizontalLine(curx1, curx2, scanlineY, p_target);
		curx1 += slope1;
		curx2 += slope2;
	}
}

void Rasterizer::DrawTopFlatTriangle(const std::vector<Vertex>& p_triangle, Texture* p_target)
{
	float slope1 = (p_triangle[2].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[2].GetPosition().y - p_triangle[0].GetPosition().y);
	float slope2 = (p_triangle[2].GetPosition().x - p_triangle[1].GetPosition().x) / (p_triangle[2].GetPosition().y - p_triangle[1].GetPosition().y);

	float curx1 = p_triangle[2].GetPosition().x, curx2 = p_triangle[2].GetPosition().x;

	for (int scanlineY = p_triangle[2].GetPosition().y; scanlineY > p_triangle[0].GetPosition().y; --scanlineY)
	{
		DrawHorizontalLine(curx1, curx2, scanlineY, p_target);
		curx1 -= slope1;
		curx2 -= slope2;
	}
}

void Rasterizer::DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target)
{
	if (x1 > x2) std::swap(x1, x2);
	for (int x = x1; x <= x2; ++x)
	{
		p_target->SetPixelColor(x, y, Color(255,255,255));
	}
}

void Rasterizer::DrawLine(int x1, int y1, int x2, int y2, Texture* p_target)
{
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
		++x1;
		SwitchFromOctantOne(octant, x1, y1);
		p_target->SetPixelColor(x1, y1, Color(255, 255, 255));
		SwitchToOctantOne(octant, x1, y1);
		e += dy;
		if (e > 0)
		{
			++y1;
			e -= dx;
		}
	}
}

void Rasterizer::SortVerticesBy(std::vector<Vertex>& p_vertices, bool x, bool y, bool z)
{
	int coord = 1;
	if (x) coord = 0;
	else if (z) coord = 2;

	std::sort(p_vertices.begin(), p_vertices.end(), [coord](const Vertex & a, const Vertex & b) -> bool
	{
		return a.GetPosition()[coord] > b.GetPosition()[coord];
	});
}

Vec4 Rasterizer::TransformPos(const Vertex& v, Mat4 transform)
{
	return transform * Vec4(v.GetPosition());
}

void Rasterizer::WorldToScreenCoord(int worldWidth, int worldHeight, 
									int screenWidth, int screenHeight, 
									const Vec3& pos, int& targetX, int& targetY)
{
	targetX = static_cast<int>(((pos.x / worldWidth) + 1) * 0.5f * screenWidth);
	targetY = static_cast<int>(screenHeight - ((pos.y / worldHeight) + 1) * 0.5f * screenHeight);
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
