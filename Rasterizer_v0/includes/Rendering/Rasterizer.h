#pragma once
#include <SceneObjects/Scene.h>
#include <Graphics/Texture.h>

using namespace SceneObjects;

namespace Rendering
{

	class Rasterizer
	{
	public:
		Rasterizer();
		~Rasterizer();

		void RenderScene(Scene* pScene, Texture* pTarget);
		void DrawTriangle(std::vector<Vertex> p_triangle, Texture* p_target);
		void DrawBottomFlatTriangle(const std::vector<Vertex>& p_triangle, Texture* p_target);
		void DrawTopFlatTriangle(const std::vector<Vertex>& p_triangle, Texture* p_target);
		void DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target);

	private:
		void DrawLine(int x1, int y1, int x2, int y2, Texture* p_target);
		void SortVerticesBy(std::vector<Vertex>& p_vertices, bool x = false, bool y = true, bool z = false);

		static Vec4 TransformPos(const Vertex& v, Mat4 transform);

		static void WorldToScreenCoord(int worldWidth, int worldHeight,
									   int screenWidth, int screenHeight,
									   const Vec3& pos, int& targetX, int& targetY);

		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}
