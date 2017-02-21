#pragma once

#ifndef _MODELBUILDER_H
#define _MODELBUILDER_H

#include <glm\vec4.hpp>
#include "graphics\structures\Mesh.h"

namespace gfx 
{
	class MeshBuilder 
	{
	protected:
		Mesh* mesh;

	public:
		MeshBuilder();
		virtual ~MeshBuilder();
		void BuildMesh(const MaterialInfo& materialInfo);
		void BuildTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
		Mesh* GetMesh();
	};
}

#endif // !_MODELBUILDER_H