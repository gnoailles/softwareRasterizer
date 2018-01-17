#pragma once
#include <SceneObjects/Scene.h>
#include <Graphics/Texture.h>

using namespace SceneObjects;

namespace Rendering
{

	class Rasterizer
	{
	private:
		float* m_depthBuffer;

	public:
		Rasterizer(unsigned int p_width, unsigned int p_height);
		~Rasterizer();

		void RenderScene(Scene* p_scene, Texture* p_target, const Mat4& p_projectionMatrix) const;

		void DrawTriangleBarycenter(const Vertex* p_triangle, Texture* p_target, const std::vector<Light>& p_lights) const;
		static void DrawWireframe(const Vertex* p_triangle, Texture* p_target);

		void DrawTriangleScanline(Vertex* p_triangle, Texture* p_target) const;
		void DrawBottomFlatTriangle(const Vertex* p_triangle, Texture* p_target) const;
		void DrawTopFlatTriangle(const Vertex* p_triangle, Texture* p_target) const;

		void DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target, Color p_color = Color(255,255,255)) const;
		static void DrawLine(int x1, int y1, int x2, int y2, Texture* p_target);

		static Mat4 CreatePerspectiveProjectionMatrix(int p_width, int p_height, float p_near, float p_far, float p_fov);

		static void WorldToScreenCoord(int worldWidth, int worldHeight,
		int screenWidth, int screenHeight,
		const Vec3& pos, int& targetX, int& targetY);
		static Vec3 WorldToScreenCoord(int worldWidth, int worldHeight,
		int screenWidth, int screenHeight, const Vec3& pos);

	private:

		inline static float EdgeFunction(const Vec3 &a, const Vec3 &b, const Vec3 &c);
		static void SortVerticesBy(Vertex* p_vertices, bool x = false, bool y = true, bool z = false);

		static Vertex TransformVertex(const Vertex& v, const Mat4& transform, const Mat4& p_projectionMatrix,const unsigned& p_width,const unsigned& p_height);

		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}