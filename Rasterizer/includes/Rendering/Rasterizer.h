#pragma once
#include <SceneObjects/Scene.h>
#include <Graphics/Texture.h>
#include "SceneObjects/Camera.h"

using namespace SceneObjects;

namespace Rendering
{
	enum
	{
		RAST_WIREFRAME			= 0x01,
		RAST_PHONG				= 0x02,
		RAST_BLINN_PHONG		= 0x04,
		RAST_PIXEL_BLINN_PHONG	= 0x08,
		RAST_ORTHO				= 0x10,
	};

	class Rasterizer
	{
	private:
		float* m_depthBuffer;
		unsigned m_viewportX;
		unsigned m_viewportY;
		unsigned m_viewportWidth;
		unsigned m_viewportHeight;

	public:
		Rasterizer(const unsigned& p_width, const unsigned& p_height);
		~Rasterizer();

		void SetViewport(const unsigned& x, const unsigned& y, const unsigned& p_width, const unsigned& p_height);

		void RenderScene(Scene* p_scene, Texture* p_target, Camera& p_camera, uint8_t& p_flags) const;

		void DrawTriangleBarycenter(Vertex* p_triangle, Scene* p_scene, Camera& p_camera, Texture* p_target, const bool& p_perPixel) const;

		static void DrawWireframe(const Vertex* p_triangle, Texture* p_target);
		static void DrawLine(const Vertex& p_vt1, const Vertex& p_vt2, Texture* p_target);

		static Mat4 CreatePerspectiveProjectionMatrix(const int& p_width, const int& p_height, const float& p_near,
		                                              const float& p_far, const float& p_fov);
		static Mat4 CreatePerspectiveProjectionMatrix(const float& p_left, const float& p_right, const float& p_bottom, const float& p_top, const float& p_near, const float& p_far);
		static Mat4 CreateOrthographicProjectionMatrix(const float& p_left, const float& p_right, const float& p_bottom, const float& p_top, const float& p_near, const float& p_far);

	private:

		static void ClipPolygon(std::vector<Vec4>& p_polygon);
		static void ClipPolygonW(std::vector<Vec4>& p_polygon);
		static void ClipPolygonXYZ(std::vector<Vec4>& p_polygon);

		static Color ComputePhong(const std::vector<Light>& p_lights, const Vertex& p_vertex,
		                          const Camera& p_camera);
		
		static Color ComputeBlinnPhong(const std::vector<Light>& p_lights, const Vertex& p_vertex,
									   const Camera& p_camera);

		inline static float EdgeFunction(const Vec3& a, const Vec3& b, const Vec3& c);
		static void SortVerticesBy(Vertex* p_vertices, bool x = false, bool y = true, bool z = false);

		static Vertex ToModelView(const Vertex& v,
		                          const Mat4& p_modelMatrix,
		                          const Mat4& p_inverseCameraMatrix);

		inline static Vec4 ToHomogeneousClipSpace(const Vec4& v,
		                       const Mat4& p_projectionMatrix);

		Vec3 ToViewport(const Vec4& v) const;

		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}
