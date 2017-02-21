#include "graphics\illumination\Lights.h"

namespace gfx 
{
	glm::vec3 Light::LightColor(const Vertex & v) const
	{
		return color / glm::pow(glm::length(LightVector(v)), 2);
	}
	
	glm::vec4 Light::ReflectedVector(const Vertex & v) const
	{
		glm::vec4 l = this->LightVector(v), n = v.Normal();
		return glm::normalize((2 * glm::dot(n, l) * n) - l);
	}
	
	PointLight::PointLight(glm::vec4 _pos, glm::vec3 _col) : Light(_col)
	{
		position = _pos;
	}

	glm::vec4 PointLight::LightVector(const Vertex & v) const
	{
		return glm::normalize(position - v.Point());
	}

	DirectionalLight::DirectionalLight(const glm::vec4 & _dir, const glm::vec3 & _col) : Light(_col)
	{
		negDirection = glm::normalize(-_dir);
	}

	glm::vec4 DirectionalLight::LightVector(const Vertex & v) const
	{
		return negDirection;
	}

	glm::vec4 SpotLight::LightVector(const Vertex& v) const
	{
		return glm::normalize(position - v.Point());
	}
	
	SpotLight::SpotLight(const glm::vec4 & _pos, const glm::vec4 & _dir, float _ref, const glm::vec3 & _col) : Light(_col)
	{
		position = _pos;
		negDirection = glm::normalize(-_dir);
		reflection = _ref;
	}

	glm::vec3 SpotLight::LightColor(const Vertex& v) const {
		return color * std::powf(glm::dot(negDirection, this->LightVector(v)), reflection) / glm::pow(glm::length(LightVector(v)), 2);
	}
}