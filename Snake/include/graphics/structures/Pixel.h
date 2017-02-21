#pragma once

#ifndef _PIXEL_H
#define _PIXEL_H

#include "graphics\structures\Vertex.h"

namespace gfx 
{
	struct Pixel 
	{
	private:
		int x, y, z;
		glm::vec3 color;
		Vertex worldVertex;

	public:
		Pixel(int _x, int _y, int _z, glm::vec3 _col, const Vertex& _worldVertex)
			: x(_x), y(_y), z(_z), color(_col), worldVertex(_worldVertex) { }
		virtual ~Pixel() { }

		int X() const { return x; }
		int Y() const { return y; }
		int Z() const { return z; }
		const glm::vec3& Color() const { return color; }
		const Vertex& WorldVertex() const { return worldVertex; }

		uint8_t R() const { return uint8_t(color.r * 255); }
		uint8_t G() const { return uint8_t(color.g * 255); }
		uint8_t B() const { return uint8_t(color.b * 255); }
		uint8_t A() const { return uint8_t(255); }
	};
}

#endif // !_PIXEL_H
