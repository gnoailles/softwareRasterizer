#include <Rendering/Rasterizer.h>
#include <algorithm>

using namespace Rendering;

Rasterizer::Rasterizer(unsigned int p_width, unsigned int p_height) 
{
	m_depthBuffer = new float[p_width * p_height];

	for (unsigned int i = 0; i < p_width * p_height; i++)
	{
		m_depthBuffer[i] = FLT_MAX;
	}
}

Rasterizer::~Rasterizer() 
{
	delete[] m_depthBuffer;
}

void Rasterizer::RenderScene(Scene* p_scene, Texture* p_target) const
{
	p_target->Clear(Color(0, 0, 0));
	for (unsigned int i = 0; i < p_target->Height() * p_target->Width(); i++)
	{
		m_depthBuffer[i] = FLT_MAX;
	}
	std::vector<Entity> entities = p_scene->GetEntities();
	for (Entity entity : entities)
	{
		const std::shared_ptr<Mesh> mesh = entity.GetMesh();
		for (unsigned i = 0; i < mesh->GetTriangleCount(); ++i)
		{
			const Vertex* triangle = mesh->GetTriangleVertices(i);
			Vertex transformedTriangle[3] = {
				TransformVertex(triangle[0], entity.GetTransformation()),
				TransformVertex(triangle[1], entity.GetTransformation()),
				TransformVertex(triangle[2], entity.GetTransformation())};

			delete[] triangle;
			triangle = nullptr;
//			DrawTriangleScanline(transformedTriangle, p_target);
//			DrawWireframe(transformedTriangle, p_target);
			DrawTriangleBarycenter(transformedTriangle, p_target, p_scene->GetLights());
		}
	}
}



void Rasterizer::DrawTriangleBarycenter(const Vertex* p_triangle, Texture* p_target, const std::vector<Light>& p_lights) const
{

	const Vec3 v0 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[0].GetPosition());
	const Vec3 v1 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[1].GetPosition());
	const Vec3 v2 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[2].GetPosition());

	unsigned int maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
	unsigned int minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
	unsigned int maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));
	unsigned int minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));

	maxX = (maxX > p_target->Width()) ? p_target->Width() : maxX;
	minX = (minX < 0) ? 0 : minX;
	maxY = (maxY > p_target->Height()) ? p_target->Height() : maxY;
	minY = (minY < 0) ? 0 : minY;

	float area = EdgeFunction(v0, v1, v2);

	for (unsigned int y = minY; y <= maxY; y++)
	{
		for (unsigned int x = minX; x <= maxX; x++)
		{
			const Vec3 p(static_cast<float>(x), static_cast<float>(y));

			float w0 = EdgeFunction(v0, v1, p);
			float w1 = EdgeFunction(v1, v2, p);
			float w2 = EdgeFunction(v2, v0, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;


				float depth = w0 * p_triangle[0].GetPosition().z + w1 * p_triangle[1].GetPosition().z + w2 * p_triangle[2].GetPosition().z;

				if (depth < m_depthBuffer[y * p_target->Width() + x])
				{
					m_depthBuffer[y * p_target->Width() + x] = depth;

					const unsigned char r = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().r + w1 * p_triangle[1].GetColor().r + w2 * p_triangle[2].GetColor().r);
					const unsigned char g = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().g + w1 * p_triangle[1].GetColor().g + w2 * p_triangle[2].GetColor().g);
					const unsigned char b = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().b + w1 * p_triangle[1].GetColor().b + w2 * p_triangle[2].GetColor().b);

					Vec3 norm = w0 * p_triangle[0].GetNormal() + w1 * p_triangle[1].GetNormal() + w2 * p_triangle[2].GetNormal();
					Color illum;
					for(Light light: p_lights)
					{
						Vec3 lm = light.GetPosition() - p;
						lm.Normalize();

						Vec3 reflection = 2 * (lm.DotProduct(norm)) * norm - lm;

						const int shininess = 6;
						float diffuse = lm.DotProduct(norm) * light.Diffuse();
						float specular = powf(reflection.DotProduct(Vec3(0.0f, 0.0f, 0.0f)), shininess) * light.Specular();

//						Color ambient(light.Ambient() *  r, light.Ambient() *  g, light.Ambient() *  b);
//						Color diffusec(, g * (lm.DotProduct(norm)) * light.Diffuse(), b * (lm.DotProduct(norm)) * light.Diffuse());
//						Color specularc(r * pow(reflection.DotProduct(Vec3(0.0f, 0.0f, 0.0f)), shininess) * light.Specular(), g * pow(reflection.DotProduct(Vec3(0.0f, 0.0f, 0.0f)), shininess) * light.Specular(), b *pow(reflection.DotProduct(Vec3(0.0f, 0.0f, 0.0f)), shininess) * light.Specular());

					
						illum = Color(r * (light.Ambient() + diffuse + specular),
									  g * (light.Ambient() + diffuse + specular),
									  b * (light.Ambient() + diffuse + specular));

					}

					p_target->SetPixelColor(x, y, illum);
				}
			}
		}
	}
}

void Rasterizer::DrawWireframe(const Vertex* p_triangle, Texture* p_target) const
{
	const Vec3 v0 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[0].GetPosition());
	const Vec3 v1 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[1].GetPosition());
	const Vec3 v2 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[2].GetPosition());

	//	WIREFRAME
	DrawLine((int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y, p_target);
	DrawLine((int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, p_target);
	DrawLine((int)v2.x, (int)v2.y, (int)v0.x, (int)v0.y, p_target);
}

void Rasterizer::DrawTriangleScanline(Vertex* p_triangle, Texture* p_target) const
{

	SortVerticesBy(p_triangle);

	const Vec3 v1 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[0].GetPosition());
	const Vec3 v2 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[1].GetPosition());
	const Vec3 v3 = WorldToScreenCoord(5, 5, p_target->Width(), p_target->Height(), p_triangle[2].GetPosition());
	
	if (v2.y == v3.y)
	{
		const Vertex triangle[3] = {v1,v2,v3};
		DrawBottomFlatTriangle(triangle, p_target);
	}
	else if (v1.y == v2.y)
	{
		const Vertex triangle[3] = { v1, v2, v3 };
		DrawTopFlatTriangle(triangle , p_target);
	}
	else
	{
		const Vec3 v4 (v1.x + (float)(v2.y - v1.y) / (float)(v3.y - v1.y) * (v3.x - v1.x) , v2.y);
		
		const Vertex bottomFlatTriangle[3] = {v1, v2, v4};
		const Vertex topFlatTriangle[3] = { v2, v4, v3 };
		DrawBottomFlatTriangle(bottomFlatTriangle, p_target);
		DrawTopFlatTriangle(topFlatTriangle, p_target);
	}

}

void Rasterizer::DrawBottomFlatTriangle(const Vertex* p_triangle, Texture* p_target) const
{
	float slope1 = (p_triangle[1].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[1].GetPosition().y - p_triangle[0].GetPosition().y);
	float slope2 = (p_triangle[2].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[2].GetPosition().y - p_triangle[0].GetPosition().y);

	unsigned int curx1 = (unsigned)p_triangle[0].GetPosition().x, curx2 = (unsigned)p_triangle[0].GetPosition().x;

	for (int scanlineY = (int)p_triangle[0].GetPosition().y; scanlineY <= p_triangle[1].GetPosition().y; ++scanlineY)
	{
		DrawHorizontalLine(curx1, curx2, scanlineY, p_target);
		curx1 += (unsigned)slope1;
		curx2 += (unsigned)slope2;
	}
}

void Rasterizer::DrawTopFlatTriangle(const Vertex* p_triangle, Texture* p_target) const
{
	float slope1 = (p_triangle[2].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[2].GetPosition().y - p_triangle[0].GetPosition().y);
	float slope2 = (p_triangle[2].GetPosition().x - p_triangle[1].GetPosition().x) / (p_triangle[2].GetPosition().y - p_triangle[1].GetPosition().y);

	unsigned int curx1 = (unsigned)p_triangle[2].GetPosition().x, curx2 = (unsigned)p_triangle[2].GetPosition().x;

	for (int scanlineY = (int)p_triangle[2].GetPosition().y; scanlineY > p_triangle[0].GetPosition().y; --scanlineY)
	{
		DrawHorizontalLine(curx1, curx2, scanlineY, p_target);
		curx1 -= (unsigned)slope1;
		curx2 -= (unsigned)slope2;
	}
}

void Rasterizer::DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target, Color p_color) const
{
	if (x1 > x2) std::swap(x1, x2);
	for (unsigned int x = x1; x <= x2; ++x)
	{
		p_target->SetPixelColor(x, y, p_color);
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

void Rasterizer::WorldToScreenCoord(const int worldWidth, const int worldHeight,
                                    const int screenWidth, const int screenHeight,
	const Vec3& pos, int& targetX, int& targetY)
{
	targetX = static_cast<int>(((pos.x / worldWidth) + 1) * 0.5f * screenWidth);
	targetY = static_cast<int>(screenHeight - ((pos.y / worldHeight) + 1) * 0.5f * screenHeight);
}

Vec3 Rasterizer::WorldToScreenCoord(int worldWidth, int worldHeight, int screenWidth, int screenHeight, const Vec3& pos)
{
	return Vec3(((pos.x / worldWidth) + 1) * 0.5f * screenWidth, screenHeight - ((pos.y / worldHeight) + 1) * 0.5f * screenHeight);
}

inline float Rasterizer::EdgeFunction(const Vec3& a, const Vec3& b, const Vec3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void Rasterizer::SortVerticesBy(Vertex* p_vertices, bool x, bool y, bool z)
{
	int coord = 1;
	if (x) coord = 0;
	else if (z) coord = 2;

	Vertex temp;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j <= 3; ++j)
		{
			if (p_vertices[i].GetPosition()[coord] > p_vertices[j].GetPosition()[coord])
			{
				temp = p_vertices[i];
				p_vertices[i] = p_vertices[j];
				p_vertices[j] = temp;
			}
		}
	}
}

Vertex Rasterizer::TransformVertex(const Vertex& v, const Mat4& transform)
{
	const Vec3 transformedPos = (transform * v.GetPosition()).ToVec3();
	const Vec3 transformedNorm = (transform.Transpose().Inverse() * Vec4(v.GetNormal(), 0)).ToVec3();
	return Vertex(transformedPos, transformedNorm, v.GetColor());
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
