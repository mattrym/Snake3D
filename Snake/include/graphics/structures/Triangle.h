#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <iostream>
#include "MaterialInfo.h"
#include "Vertex.h"

namespace gfx 
{
	struct Triangle 
	{
	private:
		Vertex* vertices;

	public:
		Triangle(const Vertex& _v1, const Vertex& _v2, const Vertex& _v3) { vertices = new Vertex[3]{ _v1, _v2, _v3 }; }
		Triangle(const Vertex* _vertices) { vertices = new Vertex[3]{ _vertices[0], _vertices[1], _vertices[2] }; }
		Triangle(const Triangle& t) { vertices = new Vertex[3]{ t.vertices[0], t.vertices[1], t.vertices[2] }; }
		virtual ~Triangle() { delete[] vertices; }

		Vertex& operator[](int i) {	return vertices[i]; }
		Vertex operator[](int i) const { return vertices[i]; }
	};
}

#endif // !_TRIANGLE_H