#include <Rendering/Rasterizer.h>
#include <algorithm>
using namespace Rendering;

Rasterizer::Rasterizer(const unsigned& p_width, const unsigned& p_height): m_viewportX(0), m_viewportY(0),
                                                                           m_viewportWidth(p_width),
                                                                           m_viewportHeight(p_height)
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

void Rasterizer::SetViewport(const unsigned& x, const unsigned& y, const unsigned& p_width, const unsigned& p_height)
{
	this->m_viewportX = x;
	this->m_viewportY = y;
	this->m_viewportWidth = p_width;
	this->m_viewportHeight = p_height;
}

void Rasterizer::RenderScene(Scene* p_scene, Texture* p_target, Camera& p_camera, uint8_t& p_flags) const
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
				ToModelView(triangle[0], entity.GetTransformation(), p_camera.GetViewMatrix()),
				ToModelView(triangle[1], entity.GetTransformation(), p_camera.GetViewMatrix()),
				ToModelView(triangle[2], entity.GetTransformation(), p_camera.GetViewMatrix())
			};

			delete[] triangle;
			triangle = nullptr;

			if (p_flags & RAST_PHONG)
			{
				transformedTriangle[0].SetColor(ComputePhong(p_scene->GetLights(), transformedTriangle[0], p_camera));
				transformedTriangle[1].SetColor(ComputePhong(p_scene->GetLights(), transformedTriangle[1], p_camera));
				transformedTriangle[2].SetColor(ComputePhong(p_scene->GetLights(), transformedTriangle[2], p_camera));
			}
			else if (p_flags & RAST_BLINN_PHONG)
			{
				transformedTriangle[0].SetColor(ComputeBlinnPhong(p_scene->GetLights(), transformedTriangle[0], p_camera));
				transformedTriangle[1].SetColor(ComputeBlinnPhong(p_scene->GetLights(), transformedTriangle[1], p_camera));
				transformedTriangle[2].SetColor(ComputeBlinnPhong(p_scene->GetLights(), transformedTriangle[2], p_camera));
			}

			std::vector<Vec4> clippedTriangle;
			clippedTriangle.reserve(3);
			clippedTriangle.emplace_back(ToHomogeneousClipSpace(Vec4(transformedTriangle[0].GetPosition(), 1),
			                                                    p_camera.GetPerspectiveProjectionMatrix()));
			clippedTriangle.emplace_back(ToHomogeneousClipSpace(Vec4(transformedTriangle[1].GetPosition(), 1),
			                                                    p_camera.GetPerspectiveProjectionMatrix()));
			clippedTriangle.emplace_back(ToHomogeneousClipSpace(Vec4(transformedTriangle[2].GetPosition(), 1),
			                                                    p_camera.GetPerspectiveProjectionMatrix()));

			// CLIP
			if (!(p_flags & RAST_ORTHO))
				ClipPolygon(clippedTriangle);

			if (clippedTriangle.size() == 0)
				continue;
			if (clippedTriangle.size() == 3)
			{
				transformedTriangle[0].SetPosition(ToViewport(clippedTriangle[0]));
				transformedTriangle[1].SetPosition(ToViewport(clippedTriangle[1]));
				transformedTriangle[2].SetPosition(ToViewport(clippedTriangle[2]));

				if (p_flags & RAST_WIREFRAME)
					DrawWireframe(transformedTriangle, p_target);
				else
					DrawTriangleBarycenter(transformedTriangle, p_scene, p_camera, p_target, (p_flags & RAST_PIXEL_BLINN_PHONG));
			}
			else
			{
				for (int j = 0; j < clippedTriangle.size() - 2; ++j)
				{
					if (!(p_flags & RAST_PIXEL_BLINN_PHONG))
					{
						transformedTriangle[0].SetPosition(ToViewport(clippedTriangle[0]));
						transformedTriangle[1].SetPosition(ToViewport(clippedTriangle[j + 1]));
						transformedTriangle[2].SetPosition(ToViewport(clippedTriangle[j + 2]));
					}

					if (p_flags & RAST_WIREFRAME)
						DrawWireframe(transformedTriangle, p_target);
					else
						DrawTriangleBarycenter(transformedTriangle, p_scene, p_camera, p_target, (p_flags & RAST_PIXEL_BLINN_PHONG));
				}
			}
		}
	}
}

void Rasterizer::DrawTriangleBarycenter(Vertex* p_triangle, Scene* p_scene, Camera& p_camera, Texture* p_target, const bool& p_perPixel) const
{
	const Vec3 v0 = p_triangle[0].GetPosition();
	const Vec3 v1 = p_triangle[1].GetPosition();
	const Vec3 v2 = p_triangle[2].GetPosition();

	int maxX = static_cast<int>(std::max(v0.x, std::max(v1.x, v2.x)));
	int minX = static_cast<int>(std::min(v0.x, std::min(v1.x, v2.x)));
	int maxY = static_cast<int>(std::max(v0.y, std::max(v1.y, v2.y)));
	int minY = static_cast<int>(std::min(v0.y, std::min(v1.y, v2.y)));

	maxX = ((unsigned)maxX > p_target->Width()) ? p_target->Width() - 1 : maxX;
	minX = (minX < 0) ? 0 : minX;
	maxY = ((unsigned)maxY > p_target->Height()) ? p_target->Height() - 1 : maxY;
	minY = (minY < 0) ? 0 : minY;

	const float area = EdgeFunction(v0, v1, v2);


	for (unsigned int y = minY; y <= (unsigned)maxY; y++)
	{
		for (unsigned int x = minX; x <= (unsigned)maxX; x++)
		{
			const Vec3 p(static_cast<float>(x), static_cast<float>(y));

			float w2 = EdgeFunction(v0, v1, p);
			float w0 = EdgeFunction(v1, v2, p);
			float w1 = EdgeFunction(v2, v0, p);

			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 /= area;
				w1 /= area;
				w2 /= area;

				const float depth = 1 / (w0 / p_triangle[0].GetPosition().z + w1 / p_triangle[1].GetPosition().z + w2 / p_triangle[2].
					GetPosition().z);

				if (depth < m_depthBuffer[y * p_target->Width() + x])
				{
					m_depthBuffer[y * p_target->Width() + x] = depth;

					const unsigned char r = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().r + w1 * p_triangle[1].
						GetColor().r + w2 * p_triangle[2].GetColor().r);
					const unsigned char g = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().g + w1 * p_triangle[1].
						GetColor().g + w2 * p_triangle[2].GetColor().g);
					const unsigned char b = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().b + w1 * p_triangle[1].
						GetColor().b + w2 * p_triangle[2].GetColor().b);
					const unsigned char a = static_cast<const unsigned char>(w0 * p_triangle[0].GetColor().a + w1 * p_triangle[1].
						GetColor().a + w2 * p_triangle[2].GetColor().a);

					if (p_perPixel)
					{
						const Vec3 norm(w0 * p_triangle[0].GetNormal() + w1 * p_triangle[1].GetNormal() + w2 * p_triangle[2].GetNormal());

						const Color illum = ComputeBlinnPhong(p_scene->GetLights(), Vertex(Vec3((float)x, (float)y, 0.f), norm, Color(r, g, b, a)), p_camera);

						p_target->SetPixelColor(x, y, illum);
					}
					else
					{
						p_target->SetPixelColor(x, y, Color(r,g,b,a));
					}
				}
			}
		}
	}
}

void Rasterizer::DrawWireframe(const Vertex* p_triangle, Texture* p_target)
{
	//	WIREFRAME
	DrawLine(p_triangle[0], p_triangle[1], p_target);
	DrawLine(p_triangle[1], p_triangle[2], p_target);
	DrawLine(p_triangle[2], p_triangle[0], p_target);
}

void Rasterizer::DrawLine(const Vertex& p_vt1, const Vertex& p_vt2, Texture* p_target)
{
	int x1 = (int)p_vt1.GetPosition().x;
	int y1 = (int)p_vt1.GetPosition().y;
	int x2 = (int)p_vt2.GetPosition().x;
	int y2 = (int)p_vt2.GetPosition().y;

	const uint8_t octant = GetLineOctant(x1, y1, x2, y2);
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
		p_target->SetPixelColor(x1, y1, p_vt1.GetColor());
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
	const float dist = p_far - p_near;
	const float scale = 1 / tanf(p_fov * 0.5f * (float)M_PI / 180.f);

	// FOV Based perspective
	return Mat4{
		scale, 0, 0, 0,
		0, scale * ratio, 0, 0,
		0, 0, -p_far / dist, -(p_far * p_near) / dist,
		0, 0, -1, 0
	};
}

Mat4 Rasterizer::CreatePerspectiveProjectionMatrix(const float& p_left, const float& p_right, const float& p_bottom,
                                                   const float& p_top, const float& p_near, const float& p_far)
{
	// Top-Bottom-Left-Right Based perspective
	return Mat4{
		2 * p_near / (p_right - p_left), 0, (p_right + p_left) / (p_right - p_left), 0,
		0, 2 * p_near / (p_top - p_bottom), (p_top + p_bottom) / (p_top - p_bottom), 0,
		0, 0, -(p_far + p_near) / (p_far - p_near) , -2 * p_far * p_near / (p_far - p_near),
		0, 0, -1, 0
	};
}

Mat4 Rasterizer::CreateOrthographicProjectionMatrix(const float& p_left, const float& p_right, const float& p_bottom,
                                                    const float& p_top, const float& p_near, const float& p_far)
{
	// Orthographic Projection
	return Mat4{
		2 / (p_right - p_left), 0, 0, -(p_right + p_left) / (p_right - p_left),
		0, 2 / (p_top - p_bottom), 0, -(p_top + p_bottom) / (p_top - p_bottom),
		0, 0, -2 / (p_far - p_near) , -(p_far + p_near) / (p_far - p_near),
		0, 0, 0, 1
	};
}

void Rasterizer::ClipPolygon(std::vector<Vec4>& p_polygon)
{
	ClipPolygonW(p_polygon);
	if (p_polygon.size() > 0)
		ClipPolygonXYZ(p_polygon);
}

void Rasterizer::ClipPolygonW(std::vector<Vec4>& p_polygon)
{
	std::vector<Vec4> input = p_polygon;

	p_polygon.clear();
	Vec4 prevVt = input.back();
	char currDot, prevDot = (prevVt.w < 0.00001f) ? -1 : 1;
	for (int i = 0; i < input.size(); ++i)
	{
		currDot = (input[i].w < 0.00001f) ? -1 : 1;
		if (prevDot * currDot < 0)
		{
			const float intersec = (0.00001f - prevVt.w) / (prevVt.w - input[i].w);
			p_polygon.emplace_back(prevVt.x + intersec * (input[i].x - prevVt.x),
			                       prevVt.y + intersec * (input[i].y - prevVt.y),
			                       prevVt.z + intersec * (input[i].z - prevVt.z),
			                       prevVt.w + intersec * (input[i].w - prevVt.w));
		}
		if (currDot > 0)
		{
			p_polygon.push_back(input[i]);
		}
		prevDot = currDot;
		prevVt = input[i];
	}
}

void Rasterizer::ClipPolygonXYZ(std::vector<Vec4>& p_polygon)
{
	for (int i = 0; i < 3; ++i)
	{
		if (p_polygon.size() == 0) return;
		std::vector<Vec4> input = p_polygon;
		p_polygon.clear();
		Vec4 prevVt = input.back();
		char prevDotHigh = (prevVt[i] <= prevVt.w) ? 1 : -1;
		char prevDotLow = (-prevVt[i] <= prevVt.w) ? 1 : -1;
		for (int j = 0; j < input.size(); ++j)
		{
			const char currDotHigh = (input[j][i] <= input[j].w) ? 1 : -1;
			const char currDotLow = (-input[j][i] <= input[j].w) ? 1 : -1;
			if (prevDotHigh * currDotHigh < 0)
			{
				const float intersec = (prevVt.w - prevVt[i]) / ((prevVt.w - prevVt[i]) - (input[j].w - input[j][i]));
				p_polygon.emplace_back(prevVt.x + intersec * (input[i].x - prevVt.x),
				                       prevVt.y + intersec * (input[i].y - prevVt.y),
				                       prevVt.z + intersec * (input[i].z - prevVt.z),
				                       prevVt.w + intersec * (input[i].w - prevVt.w));
			}

			if (prevDotLow * currDotLow < 0)
			{
				const float intersec = (prevVt.w + prevVt[i]) / ((prevVt.w + prevVt[i]) - (input[j].w + input[j][i]));
				p_polygon.emplace_back(prevVt.x + intersec * (input[i].x - prevVt.x),
				                       prevVt.y + intersec * (input[i].y - prevVt.y),
				                       prevVt.z + intersec * (input[i].z - prevVt.z),
				                       prevVt.w + intersec * (input[i].w - prevVt.w));
			}
			if (currDotHigh > 0 && currDotLow > 0)
			{
				p_polygon.push_back(input[j]);
			}
			prevDotHigh = currDotHigh;
			prevDotLow = currDotLow;
			prevVt = input[j];
		}
	}
}

Color Rasterizer::ComputePhong(const std::vector<Light>& p_lights, const Vertex& p_vertex, const Camera& p_camera)
{
	Color illum(0, 0, 0, 255);
	const int shininess = 1;

	for (Light light : p_lights)
	{
		Vec3 L = (p_camera.GetViewMatrix() * light.GetPosition()).ToVec3() - p_vertex.GetPosition();
		L.Normalize();

		Vec3 V = p_camera.GetPosition() - p_vertex.GetPosition();
		V.Normalize();

		const Vec3 R = (2 * (p_vertex.GetNormal().DotProduct(L)) * p_vertex.GetNormal()) - L;

		const float diffuse = std::clamp(p_vertex.GetNormal().DotProduct(L), 0.f, 1.f) * light.Diffuse();

		const float specular = (p_vertex.GetNormal().DotProduct(L) > 0)
			                       ? powf(std::clamp(V.DotProduct(R), 0.f, 1.f), (float)shininess) * light.Specular()
			                       : 0.f;

		illum.r += static_cast<unsigned char>(p_vertex.GetColor().r * (light.Ambient() + diffuse + specular));
		illum.g += static_cast<unsigned char>(p_vertex.GetColor().g * (light.Ambient() + diffuse + specular));
		illum.b += static_cast<unsigned char>(p_vertex.GetColor().b * (light.Ambient() + diffuse + specular));
	}
	return illum;
}

Color Rasterizer::ComputeBlinnPhong(const std::vector<Light>& p_lights, const Vertex& p_vertex, const Camera& p_camera)
{
	Color illum(0, 0, 0, 255);
	const int shininess = 1;

	for (Light light : p_lights)
	{
		Vec3 L = (p_camera.GetViewMatrix() * light.GetPosition()).ToVec3() - p_vertex.GetPosition();
		L.Normalize();

		Vec3 V = p_camera.GetPosition() - p_vertex.GetPosition();
		V.Normalize();

		Vec3 H = L + V;
		H.Normalize();

		const float diffuse = std::clamp(p_vertex.GetNormal().DotProduct(L), 0.f, 1.f) * light.Diffuse();

		const float specular = (p_vertex.GetNormal().DotProduct(L) > 0)
			                       ? powf(std::clamp(p_vertex.GetNormal().DotProduct(H), 0.f, 1.f), (float)shininess) * light.Specular()
			                       : 0.f;

		illum.r += static_cast<unsigned char>(p_vertex.GetColor().r * (light.Ambient() + diffuse + specular));
		illum.g += static_cast<unsigned char>(p_vertex.GetColor().g * (light.Ambient() + diffuse + specular));
		illum.b += static_cast<unsigned char>(p_vertex.GetColor().b * (light.Ambient() + diffuse + specular));
	}
	return illum;
}

inline float Rasterizer::EdgeFunction(const Vec3& a, const Vec3& b, const Vec3& c)
{
	return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void Rasterizer::SortVerticesBy(Vertex* p_vertices, const bool x, bool y, const bool z)
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
	Vec4 transformedVec = p_inverseCameraMatrix * p_modelMatrix * v.GetPosition();

	const Vec4 transformedNorm = (p_modelMatrix.Inverse().Transpose() * Vec4(v.GetNormal(), 0));

	return Vertex(transformedVec.ToVec3(), Vec3(transformedNorm.x, transformedNorm.y, transformedNorm.z), v.GetColor());
}

Vec4 Rasterizer::ToHomogeneousClipSpace(const Vec4& v, const Mat4& p_projectionMatrix)
{
	return (p_projectionMatrix * v);
}

Vec3 Rasterizer::ToViewport(const Vec4& v) const
{
	Vec3 homogenized = v.ToVec3();
	homogenized.x = ((homogenized.x / 2) + 1) * 0.5f * this->m_viewportWidth + this->m_viewportX;
	homogenized.y = this->m_viewportHeight - ((homogenized.y / 2) + 1) * 0.5f * this->m_viewportHeight + this->m_viewportY;

	return homogenized;
}

uint8_t Rasterizer::GetLineOctant(const int x1, const int y1, const int x2, const int y2)
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;

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
