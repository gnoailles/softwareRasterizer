#include <Graphics/Mesh.h>
#include <iostream>

using namespace Graphics;

Mesh::Mesh()
{
	std::cout << "Mesh created" << std::endl;
}


Mesh::~Mesh()
{
	std::cout << "Mesh destroyed" << std::endl;
}

unsigned Mesh::GetTriangleCount() const
{
	return (unsigned)m_indices.size() / 3;
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

std::shared_ptr<Mesh> Mesh::CreateTriangle(const Vertex& p_a, const Vertex& p_b, const Vertex& p_c)
{
	std::shared_ptr<Mesh> triangle(new Mesh);

	triangle->m_vertices.reserve(3);
	triangle->m_indices.reserve(3);

	triangle->m_vertices.push_back(p_a);
	triangle->m_vertices.push_back(p_b);
	triangle->m_vertices.push_back(p_c);

	triangle->AddTriangleIndices(0, 1, 2);

	return triangle;
}

std::shared_ptr<Mesh> Mesh::CreateCube()
{
	std::shared_ptr<Mesh> cube (new Mesh());
	cube->m_vertices.reserve(8);

	//front
	cube->m_vertices.emplace_back(-0.5f, -0.5f, 0.5f);
	cube->m_vertices.back().SetNormal(-0.5f, -0.5f, 0.5f);

	cube->m_vertices.emplace_back(0.5f, -0.5f, 0.5f);
	cube->m_vertices.back().SetNormal(0.5f, -0.5f, 0.5f);

	cube->m_vertices.emplace_back(0.5f, 0.5f, 0.5f);
	cube->m_vertices.back().SetNormal(0.5f, 0.5f, 0.5f);

	cube->m_vertices.emplace_back(-0.5f, 0.5f, 0.5f);
	cube->m_vertices.back().SetNormal(-0.5f, 0.5f, 0.5f);


	//back
	cube->m_vertices.emplace_back(-0.5f, -0.5f, -0.5f);
	cube->m_vertices.back().SetNormal(-0.5f, -0.5f, -0.5f);

	cube->m_vertices.emplace_back(0.5f, -0.5f, -0.5f);
	cube->m_vertices.back().SetNormal(0.5f, -0.5f, -0.5f);

	cube->m_vertices.emplace_back(0.5f, 0.5f, -0.5f);
	cube->m_vertices.back().SetNormal(0.5f, 0.5f, -0.5f);

	cube->m_vertices.emplace_back(-0.5f, 0.5f, -0.5f);
	cube->m_vertices.back().SetNormal(-0.5f, 0.5f, -0.5f);


	cube->m_indices.reserve(36);

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

std::shared_ptr<Mesh> Mesh::CreateSphere(const int& p_latitudeCount, const int& p_longitudeCount)
{
	std::shared_ptr<Mesh>sphere(new Mesh);

	sphere->m_vertices.reserve(p_latitudeCount * p_longitudeCount);
	sphere->m_indices.reserve(p_latitudeCount * p_latitudeCount * 2);

	const float radius = 0.5f;

	for (double latNumber = 0; latNumber <= p_latitudeCount; latNumber++) {
		const double theta = latNumber * M_PI / p_latitudeCount;
		const double sinTheta = sin(theta);
		const double cosTheta = cos(theta);

		for (double longNumber = 0; longNumber <= p_longitudeCount; longNumber++) {
			const double phi = longNumber * 2 * M_PI / p_longitudeCount;
			const double sinPhi = sin(phi);
			const double cosPhi = cos(phi);

			const float x = (float)(cosPhi * sinTheta);
			const float z = (float)(sinPhi * sinTheta);

			sphere->m_vertices.emplace_back(x * radius, (float)cosTheta * radius, z * radius);
			sphere->m_vertices.back().SetNormal(x, (float)cosTheta, z);
		}
	}
	for (int latNumber2 = 0; latNumber2 < p_latitudeCount; latNumber2++) {
		for (int longNumber = 0; longNumber < p_longitudeCount; longNumber++) {
			const int first = (latNumber2 * (p_longitudeCount + 1)) + longNumber;
			const int second = first + p_longitudeCount + 1;

			sphere->AddTriangleIndices(first, second, first + 1);
			sphere->AddTriangleIndices(second, second + 1, first + 1);
		}
	}

	return sphere;
}

void Mesh::SetColor(const Color & p_color)
{
	for (Vertex& vertex : m_vertices)
	{
		vertex.SetColor(p_color);
	}
}

void Mesh::AddTriangleIndices(const int p_idx1, const int p_idx2, const int p_idx3)
{
	this->m_indices.push_back(p_idx1);
	this->m_indices.push_back(p_idx2);
	this->m_indices.push_back(p_idx3);
}