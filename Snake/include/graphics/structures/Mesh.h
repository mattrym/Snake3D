#pragma once

#ifndef _MESH_H
#define _MESH_H

#include <iostream>
#include <vector>
#include <glm\mat4x4.hpp>
#include "Triangle.h"

namespace gfx 
{
	class Mesh 
	{
	protected:
		std::vector<Triangle> triangles;
		MaterialInfo materialInfo;

	public:
		Mesh(const MaterialInfo& _materialInfo) : materialInfo(_materialInfo) { }
		virtual ~Mesh() { }

		MaterialInfo Material() const { return materialInfo; }
		std::vector<Triangle>::const_iterator TrianglesBegin() const { return triangles.cbegin(); }
		std::vector<Triangle>::const_iterator TrianglesEnd() const { return triangles.cend(); }

		friend class MeshBuilder;
	};
}

#endif // !_MESH_H
