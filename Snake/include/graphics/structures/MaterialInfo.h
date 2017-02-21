#pragma once

#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <glm\vec3.hpp>

namespace gfx 
{
	struct MaterialInfo 
	{
	protected:
		float n;
		glm::vec3 ambient, diffuse, specular;
	public:
		MaterialInfo() : n(0), ambient(glm::vec3(0, 0, 0)), diffuse(glm::vec3(0, 0, 0)), specular(glm::vec3(0, 0, 0)) { }
		MaterialInfo(float _n, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular)
			: n(_n), ambient(_ambient), diffuse(_diffuse), specular(_specular) { }
		virtual ~MaterialInfo() { }

		float N() const { return n; }
		glm::vec3 Ambient() const { return ambient; }
		glm::vec3 Diffuse() const { return diffuse; }
		glm::vec3 Specular() const { return specular; }
	};
}

#endif // !_MATERIAL_H


