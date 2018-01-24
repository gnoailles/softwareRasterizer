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
		Rasterizer(const unsigned& p_width, const unsigned& p_height);
		~Rasterizer();

		void RenderScene(Scene* p_scene, Texture* p_target, const Mat4& p_projectionMatrix,
		                 const Mat4& p_inverseCameraMatrix) const;

		void DrawTriangleBarycenter(Vertex* p_triangle, const Mat4& p_projectionMatrix, const Mat4& p_inverseCameraMatrix,
		                            const std::vector<Light>& p_lights, Texture* p_target) const;
		static void DrawWireframe(const Vertex* p_triangle, Texture* p_target);

		void DrawTriangleScanline(Vertex* p_triangle, Texture* p_target) const;
		void DrawBottomFlatTriangle(const Vertex* p_triangle, Texture* p_target) const;
		void DrawTopFlatTriangle(const Vertex* p_triangle, Texture* p_target) const;

		void DrawHorizontalLine(unsigned int x1, unsigned int x2, unsigned int y, Texture* p_target,
		                        Color p_color = Color(255, 255, 255)) const;
		static void DrawLine(int x1, int y1, int x2, int y2, Texture* p_target);

		static Mat4 CreatePerspectiveProjectionMatrix(const int& p_width, const int& p_height, const float& p_near,
		                                              const float& p_far, const float& p_fov);

	private:

		static Color ComputePhongIllumination(const std::vector<Light>& p_lights, const Vertex& p_vertex,
		                                      const Mat4& p_inverseCameraMatrix);

		inline static float EdgeFunction(const Vec3& a, const Vec3& b, const Vec3& c);
		static void SortVerticesBy(Vertex* p_vertices, bool x = false, bool y = true, bool z = false);

		static Vertex ToModelView(const Vertex& v,
		                          const Mat4& p_modelMatrix,
		                          const Mat4& p_inverseCameraMatrix);

		static Vertex ToScreen(const Vertex& v,
		                       const Mat4& p_projectionMatrix,
		                       const unsigned& p_width,
		                       const unsigned& p_height);

		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}
