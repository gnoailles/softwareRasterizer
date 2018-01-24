#include <Rendering/Rasterizer.h>
#include <algorithm>

using namespace Rendering;

Rasterizer::Rasterizer(const unsigned& p_width, const unsigned& p_height)
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

void Rasterizer::RenderScene(Scene* p_scene, Texture* p_target, const Mat4& p_projectionMatrix,
                             const Mat4& p_inverseCameraMatrix) const
{
	p_target->Clear(Color(0, 0, 0));
	for (unsigned int i = 0; i < p_target->Height() * p_target->Width(); i++)
	{
		m_depthBuffer[i] = FLT_MAX;
	}

	for (Entity entity : p_scene->GetEntities())
	{
		const std::shared_ptr<Mesh> mesh = entity.GetMesh();
		for (unsigned i = 0; i < mesh->GetTriangleCount(); ++i)
		{
			const Vertex* triangle = mesh->GetTriangleVertices(i);
			Vertex transformedTriangle[3] = {
				ToModelView(triangle[0], entity.GetTransformation(), p_inverseCameraMatrix),
				ToModelView(triangle[1], entity.GetTransformation(), p_inverseCameraMatrix),
				ToModelView(triangle[2], entity.GetTransformation(), p_inverseCameraMatrix)
			};

			delete[] triangle;
			triangle = nullptr;

			//						DrawTriangleScanline(transformedTriangle, p_target);
			//						DrawWireframe(transformedTriangle, p_target);
			DrawTriangleBarycenter(transformedTriangle, p_projectionMatrix, p_inverseCameraMatrix, p_scene->GetLights(), p_target);

			//DEBUG normals
//			DrawLine(transformedTriangle[0].GetPosition().x, transformedTriangle[0].GetPosition().y,
//			         transformedTriangle[0].GetPosition().x + transformedTriangle[0].GetNormal().x * 50,
//			         transformedTriangle[0].GetPosition().y + transformedTriangle[0].GetNormal().y * 50, p_target);
//
//			DrawLine(transformedTriangle[1].GetPosition().x, transformedTriangle[1].GetPosition().y,
//			         transformedTriangle[1].GetPosition().x + transformedTriangle[1].GetNormal().x * 50,
//			         transformedTriangle[1].GetPosition().y + transformedTriangle[1].GetNormal().y * 50, p_target);
//
//			DrawLine(transformedTriangle[2].GetPosition().x, transformedTriangle[2].GetPosition().y,
//			         transformedTriangle[2].GetPosition().x + transformedTriangle[2].GetNormal().x * 50,
//			         transformedTriangle[2].GetPosition().y + transformedTriangle[2].GetNormal().y * 50, p_target);
		}
	}
}

void Rasterizer::DrawTriangleBarycenter(Vertex* p_triangle, const Mat4& p_projectionMatrix,
                                        const Mat4& p_inverseCameraMatrix,
                                        const std::vector<Light>& p_lights, Texture* p_target) const
{
	const Color v0Illum = ComputePhongIllumination(p_lights, p_triangle[0], p_inverseCameraMatrix);
	const Color v1Illum = ComputePhongIllumination(p_lights, p_triangle[1], p_inverseCameraMatrix);
	const Color v2Illum = ComputePhongIllumination(p_lights, p_triangle[2], p_inverseCameraMatrix);

	p_triangle[0] = ToScreen(p_triangle[0], p_projectionMatrix, p_target->Width(), p_target->Height());
	p_triangle[1] = ToScreen(p_triangle[1], p_projectionMatrix, p_target->Width(), p_target->Height());
	p_triangle[2] = ToScreen(p_triangle[2], p_projectionMatrix, p_target->Width(), p_target->Height());

	const Vec3 v0 = p_triangle[0].GetPosition();
	const Vec3 v1 = p_triangle[1].GetPosition();
	const Vec3 v2 = p_triangle[2].GetPosition();

	unsigned int maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
	unsigned int minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
	unsigned int maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));
	unsigned int minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));

	maxX = (maxX > p_target->Width()) ? p_target->Width() : maxX;
	minX = (minX < 0) ? 0 : minX;
	maxY = (maxY > p_target->Height()) ? p_target->Height() : maxY;
	minY = (minY < 0) ? 0 : minY;

	const float area = EdgeFunction(v0, v1, v2);


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

				float depth = 1 / (w0 / p_triangle[0].GetPosition().z + w1 / p_triangle[1].GetPosition().z + w2 / p_triangle[2].
					GetPosition().z);

				if (depth < m_depthBuffer[y * p_target->Width() + x])
				{	
					m_depthBuffer[y * p_target->Width() + x] = depth;

					const unsigned char r = static_cast<const unsigned char>(w0 * v0Illum.r + w1 * v1Illum.r + w2 * v2Illum.r);
					const unsigned char g = static_cast<const unsigned char>(w0 * v0Illum.g + w1 * v1Illum.g + w2 * v2Illum.g);
					const unsigned char b = static_cast<const unsigned char>(w0 * v0Illum.b + w1 * v1Illum.b + w2 * v2Illum.b);
					const unsigned char a = static_cast<const unsigned char>(w0 * v0Illum.a + w1 * v1Illum.a + w2 * v2Illum.a);
					//
					//										const unsigned char r = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().r + w1 * p_triangle[1].GetColor().r + w2 * p_triangle[3].GetColor().r);
					//										const unsigned char g = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().g + w1 * p_triangle[1].GetColor().g + w2 * p_triangle[3].GetColor().g);
					//										const unsigned char b = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().b + w1 * p_triangle[1].GetColor().b + w2 * p_triangle[3].GetColor().b);
					//										const unsigned char a = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().a + w1 * p_triangle[1].GetColor().a + w2 * p_triangle[3].GetColor().a);

					p_target->SetPixelColor(x, y, Color(r, g, b, a));
				}
			}
		}
	}
}

void Rasterizer::DrawWireframe(const Vertex* p_triangle, Texture* p_target)
{
	//	WIREFRAME
	DrawLine((int)p_triangle[0].GetPosition().x, (int)p_triangle[0].GetPosition().y, (int)p_triangle[1].GetPosition().x,
	         (int)p_triangle[1].GetPosition().y, p_target);
	DrawLine((int)p_triangle[1].GetPosition().x, (int)p_triangle[1].GetPosition().y, (int)p_triangle[2].GetPosition().x,
	         (int)p_triangle[2].GetPosition().y, p_target);
	DrawLine((int)p_triangle[2].GetPosition().x, (int)p_triangle[2].GetPosition().y, (int)p_triangle[0].GetPosition().x,
	         (int)p_triangle[0].GetPosition().y, p_target);
}

void Rasterizer::DrawTriangleScanline(Vertex* p_triangle, Texture* p_target) const
{
	SortVerticesBy(p_triangle);

	const Vec3 v1 = p_triangle[0].GetPosition();
	const Vec3 v2 = p_triangle[1].GetPosition();
	const Vec3 v3 = p_triangle[2].GetPosition();

	if (v2.y == v3.y)
	{
		const Vertex triangle[3] = {v1,v2,v3};
		DrawBottomFlatTriangle(triangle, p_target);
	}
	else if (v1.y == v2.y)
	{
		const Vertex triangle[3] = {v1, v2, v3};
		DrawTopFlatTriangle(triangle, p_target);
	}
	else
	{
		const Vec3 v4(v1.x + (float)(v2.y - v1.y) / (float)(v3.y - v1.y) * (v3.x - v1.x), v2.y);

		const Vertex bottomFlatTriangle[3] = {v1, v2, v4};
		const Vertex topFlatTriangle[3] = {v2, v4, v3};
		DrawBottomFlatTriangle(bottomFlatTriangle, p_target);
		DrawTopFlatTriangle(topFlatTriangle, p_target);
	}
}

void Rasterizer::DrawBottomFlatTriangle(const Vertex* p_triangle, Texture* p_target) const
{
	float slope1 = (p_triangle[1].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[1].GetPosition().y -
		p_triangle[0].GetPosition().y);
	float slope2 = (p_triangle[2].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[2].GetPosition().y -
		p_triangle[0].GetPosition().y);

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
	float slope1 = (p_triangle[2].GetPosition().x - p_triangle[0].GetPosition().x) / (p_triangle[2].GetPosition().y -
		p_triangle[0].GetPosition().y);
	float slope2 = (p_triangle[2].GetPosition().x - p_triangle[1].GetPosition().x) / (p_triangle[2].GetPosition().y -
		p_triangle[1].GetPosition().y);

	unsigned int curx1 = (unsigned)p_triangle[2].GetPosition().x, curx2 = (unsigned)p_triangle[2].GetPosition().x;

	for (int scanlineY = (int)p_triangle[2].GetPosition().y; scanlineY > p_triangle[0].GetPosition().y; --scanlineY)
	{
		DrawHorizontalLine(curx1, curx2, scanlineY, p_target);
		curx1 -= (unsigned)slope1;
		curx2 -= (unsigned)slope2;
	}
}

void Rasterizer::DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target,
                                    Color p_color) const
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

Mat4 Rasterizer::CreatePerspectiveProjectionMatrix(const int& p_width, const int& p_height, const float& p_near,
                                                   const float& p_far, const float& p_fov)
{
	const float ratio = p_width / (float)p_height;
	const float top = tan(p_fov * 0.5 * M_PI / 180) * p_near;
	const float right = top * ratio;
	const float dist = p_far - p_near;

	// FOV Based perspective
	//	const float scale = 1 / tan(p_fov * 0.5 * M_PI / 180);
	//	return Mat4{scale,	0,		0,							0,
	//				0,		scale * ratio,	0,					0,
	//				0,		0,		-p_far / dist,				-1,
	//				0,		0,		-(p_far * p_near) / dist,	0};

	// Top-Bottom-Left-Right Based perspective
	return Mat4{
		p_near / right, 0, 0, 0,
		0, p_near / top, 0, 0,
		0, 0, -(p_far + p_near) / dist, -1,
		0, 0, -2 * p_far * p_near / dist, 0
	};
}

Color Rasterizer::ComputePhongIllumination(const std::vector<Light>& p_lights, const Vertex& p_vertex,
                                           const Mat4& p_inverseCameraMatrix)
{
	Color illum(0,0,0,0);
	for (Light light : p_lights)
	{
		Vec3 L = (p_inverseCameraMatrix * light.GetPosition()).ToVec3() - p_vertex.GetPosition();
		L.Normalize();

		Vec3 V = (p_inverseCameraMatrix.Inverse() * Vec4(0.0, 0.0, 0.0)).ToVec3() - p_vertex.GetPosition();
		V.Normalize();

		const int shininess = 1;
//		Vec3 R = (2 * (p_vertex.GetNormal().DotProduct(L)) * p_vertex.GetNormal()) - L;

		Vec3 H = L + V;
		H.Normalize();

		const float diffuse = std::clamp(p_vertex.GetNormal().DotProduct(L), 0.f, 1.f) * light.Diffuse();

		const float specular = (p_vertex.GetNormal().DotProduct(L) > 0) ? powf(p_vertex.GetNormal().DotProduct(H), shininess) * light.Specular() : 0;

		illum.r += static_cast<unsigned char>(p_vertex.GetColor().r * (light.Ambient() + diffuse + specular));
		illum.g += static_cast<unsigned char>(p_vertex.GetColor().g * (light.Ambient() + diffuse + specular));
		illum.b += static_cast<unsigned char>(p_vertex.GetColor().b * (light.Ambient() + diffuse + specular));
		illum.a += static_cast<unsigned char>(p_vertex.GetColor().a * (light.Ambient() + diffuse + specular));
	}
	return illum;
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

Vertex Rasterizer::ToModelView(const Vertex& v,
                               const Mat4& p_modelMatrix,
                               const Mat4& p_inverseCameraMatrix)
{
	Mat4 transform = p_inverseCameraMatrix * p_modelMatrix;
	Vec4 transformedVec = transform * v.GetPosition();

	const Vec3 transformedNorm = (transform.Inverse().Transpose() * Vec4(v.GetNormal(), 0)).ToVec3();

	return Vertex(transformedVec.ToVec3(), transformedNorm, v.GetColor());
}

Vertex Rasterizer::ToScreen(const Vertex& v, const Mat4& p_projectionMatrix, const unsigned& p_width,
                            const unsigned& p_height)
{
	Vec4 transformedVec = p_projectionMatrix * v.GetPosition();
	transformedVec.Homogenize();
	transformedVec.x = ((transformedVec.x / 2) + 1) * 0.5f * p_width;
	transformedVec.y = p_height - ((transformedVec.y / 2) + 1) * 0.5f * p_height;

	return Vertex(transformedVec.ToVec3(), v.GetNormal(), v.GetColor());
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
	case 1: // X,Y
		return;
	case 2: // Y,X
		std::swap(x, y);
		break;
	case 3: // Y,-X
		x = -x;
		std::swap(x, y);

		break;
	case 4: // -X,Y
		x = -x;
		break;
	case 5: // -X,-Y
		x = -x;
		y = -y;
		break;
	case 6: // -Y,-X
		x = -x;
		y = -y;
		std::swap(x, y);

		break;
	case 7: // -Y,X
		y = -y;
		std::swap(x, y);

		break;
	case 8: // X,-Y
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
	case 1: // X,Y
		return;
	case 2: // Y,X
		std::swap(x, y);
		break;
	case 3: // -Y,X
		y = -y;
		std::swap(x, y);
		break;
	case 4: // -X,Y
		x = -x;
		break;
	case 5: // -X,-Y
		x = -x;
		y = -y;
		break;
	case 6: // -Y,-X
		x = -x;
		y = -y;
		std::swap(x, y);
		break;
	case 7: // Y,-X
		x = -x;
		std::swap(x, y);
		break;
	case 8: // X,-Y
		y = -y;
		break;
	default:
		break;
	}
}
