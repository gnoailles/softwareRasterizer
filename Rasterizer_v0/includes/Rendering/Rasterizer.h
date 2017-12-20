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
		void DrawTriangle(const Vertex*, Texture* pTarget);
		void DrawLine(int x1, int y1, int x2, int y2, Texture* pTarget);

	private:
		static Vec4 TransformPos(const Vertex& v, Mat4 transform);

		static void WorldToScreenCoord(int worldWidth, int worldHeight,
									   int screenWidth, int screenHeight,
									   const Vec3& pos, int& targetX, int& targetY);

		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}
