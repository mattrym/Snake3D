#pragma once

#ifndef _ILLUMINATORS_H
#define _ILLUMINATORS_H

#include <vector>
#include "Lights.h"
#include "graphics\structures\MaterialInfo.h"
#include "graphics\structures\Vertex.h"
#include "graphics\Camera.h"

namespace gfx 
{
	class Reflector
	{
	protected:
		Camera*& camera;
		const std::vector<Light*>& lights;

	public:
		Reflector(const std::vector<Light*>& _lights, Camera*& _camera) : lights(_lights), camera(_camera) { }
		virtual ~Reflector() { }
		virtual glm::vec3 Reflect(const Vertex& vertex, const MaterialInfo& mInfo) = 0;
	};

	class PhongReflector : public Reflector
	{
	public:
		PhongReflector(const std::vector<Light*>& _lights, Camera*& _camera) : Reflector(_lights, _camera) { }
		virtual ~PhongReflector() { }
		virtual glm::vec3 Reflect(const Vertex& vertex, const MaterialInfo& mInfo);
	};

	class BlinnReflector : public Reflector
	{
	public:
		BlinnReflector(const std::vector<Light*>& _lights, Camera*& _camera) : Reflector(_lights, _camera) { }
		virtual ~BlinnReflector() { }
		virtual glm::vec3 Reflect(const Vertex& vertex, const MaterialInfo& mInfo);
	};
}

#endif // !_ILLUMINATORS_H