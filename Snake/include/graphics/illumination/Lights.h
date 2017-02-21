#pragma once

#ifndef _LIGHTS_H
#define _LIGHTS_H

#include <glm\vec4.hpp>
#include "graphics\structures\Vertex.h"

namespace gfx 
{
	class Light 
	{
	protected:
		glm::vec3 color;
	
	public:
		Light(glm::vec3 _color) : color(_color) { }
		virtual ~Light() { }

		virtual glm::vec3 LightColor(const Vertex& v) const;
		virtual glm::vec4 LightVector(const Vertex& v) const = 0;
		glm::vec4 ReflectedVector(const Vertex& v) const;
	};

	class PointLight : public Light 
	{
	private:
		glm::vec4 position;
	
	public:
		PointLight(glm::vec4 _pos, glm::vec3 _col);
		virtual ~PointLight() { }
		virtual glm::vec4 LightVector(const Vertex& v) const;
	};

	class DirectionalLight : public Light 
	{
	private:
		glm::vec4 negDirection;
	
	public:
		DirectionalLight(const glm::vec4& _dir, const glm::vec3& _col);
		virtual ~DirectionalLight() { }
		virtual glm::vec4 LightVector(const Vertex& v) const;
	};

	class SpotLight : public Light 
	{
	private:
		glm::vec4 position;
		glm::vec4 negDirection;
		float reflection;

	public:
		SpotLight(const glm::vec4& _pos, const glm::vec4& _dir, float _ref, const glm::vec3& _col);
		virtual ~SpotLight() { }
		virtual glm::vec3 LightColor(const Vertex& v) const;
		virtual glm::vec4 LightVector(const Vertex& v) const;
	};
}

#endif // !_LIGHTS_H