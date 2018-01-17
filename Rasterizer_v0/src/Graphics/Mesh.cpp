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

std::shared_ptr<Mesh> Mesh::CreateCube()
{
	std::shared_ptr<Mesh> cube (new Mesh());
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

	//Normals
	for (int i = 0; i < 36; i += 3)
	{
		const Vec3 v0 = cube->m_vertices[cube->m_indices[i]].GetPosition() - cube->m_vertices[cube->m_indices[i+1]].GetPosition();
		const Vec3 v1 = cube->m_vertices[cube->m_indices[i+1]].GetPosition() - cube->m_vertices[cube->m_indices[i+2]].GetPosition();
		const Vec3 v2 = cube->m_vertices[cube->m_indices[i+2]].GetPosition() - cube->m_vertices[cube->m_indices[i]].GetPosition();

		Vec3 n0 = v0.CrossProduct(v2);
		n0.Normalize();
		Vec3 n1 = v1.CrossProduct(v0);
		n1.Normalize();
		Vec3 n2 = v2.CrossProduct(v1);
		n2.Normalize();

		cube->m_vertices[cube->m_indices[i]].SetNormal(cube->m_vertices[cube->m_indices[i]].GetNormal() + n1);
		cube->m_vertices[cube->m_indices[i+1]].SetNormal(cube->m_vertices[cube->m_indices[i+1]].GetNormal() + n1);
		cube->m_vertices[cube->m_indices[i+2]].SetNormal(cube->m_vertices[cube->m_indices[i+2]].GetNormal() + n2);
		
	}

	for (int i = 0; i < 8; ++i)
	{
		cube->m_vertices[i].SetNormal(cube->m_vertices[i].GetNormal().Normalize());
	}
	return cube;
}

std::shared_ptr<Mesh> Mesh::CreateSphere(const int& p_latitudeCount, const int& p_longitudeCount)
{
	std::shared_ptr<Mesh>sphere(new Mesh);

	sphere->m_vertices.reserve(p_latitudeCount * p_longitudeCount);
	sphere->m_indices.reserve(p_latitudeCount * p_latitudeCount * 2);

	for (double latNumber = 0; latNumber <= p_latitudeCount; latNumber++) {
		double theta = latNumber * M_PI / p_latitudeCount;
		double sinTheta = sin(theta);
		double cosTheta = cos(theta);

		for (double longNumber = 0; longNumber <= p_longitudeCount; longNumber++) {

			double phi = longNumber * 2 * M_PI / p_longitudeCount;
			double sinPhi = sin(phi);
			double cosPhi = cos(phi);

			float x = (float)(cosPhi * sinTheta);
			float z = (float)(sinPhi * sinTheta);

			sphere->m_vertices.emplace_back(x, cosTheta, z);
			sphere->m_vertices.back().SetNormal(x, cosTheta, z);
		}
	}
	for (int latNumber2 = 0; latNumber2 < p_latitudeCount; latNumber2++) {
		for (int longNumber = 0; longNumber < p_longitudeCount; longNumber++) {

			int first = (latNumber2 * (p_longitudeCount + 1)) + longNumber;
			int second = first + p_longitudeCount + 1;

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