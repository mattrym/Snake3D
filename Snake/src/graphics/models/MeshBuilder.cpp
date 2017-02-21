#include <glm\gtc\matrix_transform.hpp>
#include "graphics\models\MeshBuilder.h"

namespace gfx 
{
	MeshBuilder::MeshBuilder() 
	{
		mesh = nullptr;
	}

	MeshBuilder::~MeshBuilder() 
	{
		if (mesh)
		{
			delete mesh;
		}
	}

	void MeshBuilder::BuildMesh(const MaterialInfo& materialInfo)
	{
		mesh = new Mesh(materialInfo);
	}

	void MeshBuilder::BuildTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) 
	{
		mesh->triangles.push_back(Triangle(v1, v2, v3));
	}

	Mesh* MeshBuilder::GetMesh() 
	{
		Mesh* newMesh = mesh;
		mesh = nullptr;
		return newMesh;
	}
}
