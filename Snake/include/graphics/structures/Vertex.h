#pragma once

#ifndef _VERTEX_H
#define _VERTEX_H

#include <iostream>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

namespace gfx 
{
	struct Vertex 
	{
	private:
		glm::vec4 point;
		glm::vec4 normal;
	public:
		Vertex(glm::vec4 _point, glm::vec4 _normal) : point(_point), normal(_normal) { }
		virtual ~Vertex() { }
		bool operator==(const Vertex& v) const { return point == v.point && normal == v.normal; }
		
		float X() const { return point[0]; }
		float Y() const { return point[1]; }
		float Z() const { return point[2]; }

		glm::vec4& Point() { return point; }
		glm::vec4& Normal() { return normal; }
		glm::vec4 Point() const { return point; }
		glm::vec4 Normal() const { return normal; }
		
		friend Vertex operator*(const glm::mat4& m, const Vertex& v) { return Vertex(m * v.point, m * v.normal); }
	};
}

#endif // !_VERTEX_H