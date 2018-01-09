#include <Graphics/Mesh.h>

using namespace Graphics;

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

unsigned Mesh::GetTriangleCount() const
{
	return m_indices.size() / 3;
}

const Vertex* Mesh::GetTriangleVertices(unsigned triangleIndex) const
{
	triangleIndex *= 3;
	Vertex* triangle = new Vertex[3];
	triangle[0] = this->m_vertices[this->m_indices[triangleIndex]];
	triangle[1] = this->m_vertices[this->m_indices[triangleIndex + 1]];
	triangle[2] = this->m_vertices[this->m_indices[triangleIndex + 2]];
	return triangle;
}

Mesh* Mesh::CreateCube()
{
	Mesh* cube = new Mesh();
	cube->m_vertices.reserve(8);

	//front
	cube->m_vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	cube->m_vertices.emplace_back(0.5f, -0.5f, 0.5f);
	cube->m_vertices.emplace_back(0.5f, 0.5f, 0.5f);
	cube->m_vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	//back
	cube->m_vertices.emplace_back(-0.5f, -0.5f, -0.5f);
	cube->m_vertices.emplace_back(0.5f, -0.5f, -0.5f);
	cube->m_vertices.emplace_back(0.5f, 0.5f, -0.5f);
	cube->m_vertices.emplace_back(-0.5f, 0.5f, -0.5f);

	cube->m_indices.reserve(36);

	//{m_position={x=-1.96515512 y=-0.497457892 z=2.46270013 } }

	// front
	cube->AddTriangleIndices(0,1,2);
	cube->AddTriangleIndices(2,3,0);

	// top
	cube->AddTriangleIndices(1,5,6);
	cube->AddTriangleIndices(6,2,1);

	// back
	cube->AddTriangleIndices(7,6,5);
	cube->AddTriangleIndices(5,4,7);

	// bottom
	cube->AddTriangleIndices(4,0,3);
	cube->AddTriangleIndices(3,7,4);

	// left
	cube->AddTriangleIndices(4,5,1);
	cube->AddTriangleIndices(1,0,4);

	// right
	cube->AddTriangleIndices(3,2,6);
	cube->AddTriangleIndices(6,7,3);

	return cube;
}

Mesh* Mesh::CreateSphere(const int& p_latitudeCount, const int& p_longitudeCount)
{
	Mesh* sphere = new Mesh;

	//TODO implement Coord & Indices

	return sphere;
}

void Mesh::AddTriangleIndices(const int p_idx1, const int p_idx2, const int p_idx3)
{
	this->m_indices.push_back(p_idx1);
	this->m_indices.push_back(p_idx2);
	this->m_indices.push_back(p_idx3);
}