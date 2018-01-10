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
		void DrawTriangleBarycenter(std::vector<Vertex> p_triangle, Texture* p_target) const;
		void DrawTriangleScanline(std::vector<Vertex> p_triangle, Texture* p_target);
		void DrawBottomFlatTriangle(const std::vector<Vertex>& p_triangle, Texture* p_target);
		void DrawTopFlatTriangle(const std::vector<Vertex>& p_triangle, Texture* p_target);
		void DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target, Color p_color = Color(255,255,255)) const;

		static void WorldToScreenCoord(int worldWidth, int worldHeight,
			int screenWidth, int screenHeight,
			const Vec3& pos, int& targetX, int& targetY);
		static Vec3 WorldToScreenCoord(int worldWidth, int worldHeight,
			int screenWidth, int screenHeight, const Vec3& pos);

	private:
		static void DrawLine(int x1, int y1, int x2, int y2, Texture* p_target);
		inline static float EdgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c);
		void SortVerticesBy(std::vector<Vertex>& p_vertices, bool x = false, bool y = true, bool z = false);

		static Vec4 TransformPos(const Vertex& v, Mat4 transform);

		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}