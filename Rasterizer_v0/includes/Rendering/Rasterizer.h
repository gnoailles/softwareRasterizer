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
		void DrawLine(int x1, int y1, int x2, int y2, Texture* pTarget);
		static uint8_t GetLineOctant(int x1, int y1, int x2, int y2);

	private:
		static void SwitchToOctantOne(const uint8_t octant, int& x, int& y);
		static void SwitchFromOctantOne(const uint8_t octant, int& x, int& y);
	};
}
