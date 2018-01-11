#pragma once
#include <vector>
#include <Graphics/Vertex.h>

using namespace Maths;

namespace Graphics
{
	class Mesh
	{
	private:
		std::vector<Vertex> m_vertices;
		std::vector<int> m_indices;
	public:
		Mesh();
		~Mesh();

		unsigned GetTriangleCount() const;
		const Vertex* GetTriangleVertices(unsigned triangleIndex) const;

		static Mesh* CreateCube();
		static Mesh* CreateSphere(const int& p_latitudeCount, const int& p_longitudeCount);
		void SetColor(const Color& p_color);

	private:
		void AddTriangleIndices(const int p_idx1,const int p_idx2,const int p_idx3);
	};
}
