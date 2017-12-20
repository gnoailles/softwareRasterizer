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
		Vertex* GetTriangleVertices(unsigned triangleIndex);

		static Mesh* CreateCube();
		static Mesh* CreateSphere(const int& p_latitudeCount, const int& p_longitudeCount);

	private:
		void AddTriangleIndices(const int p_idx1,const int p_idx2,const int p_idx3);
	};
}
