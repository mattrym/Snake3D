#pragma once

#ifndef _SHADERS_H
#define _SHADERS_H

#include <vector>
#include "graphics\structures\MaterialInfo.h"
#include "graphics\structures\Pixel.h"
#include "graphics\Camera.h"
#include "Lights.h"

namespace gfx 
{
	class Shader 
	{
	protected:
		const std::vector<Light*>& lights;
		Camera*& camera;

	public:
		Shader(const std::vector<Light*>& _lights, Camera*& _camera) : lights(_lights), camera(_camera) {}
		virtual ~Shader() { }
		virtual Pixel ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo) = 0;
	};

	class PlainShader : public Shader 
	{
	public:
		PlainShader(const std::vector<Light*>& _lights, Camera*& _camera) : Shader(_lights, _camera) { }
		virtual ~PlainShader() { }
		virtual Pixel ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo);
	};

	class GouraudShader : public Shader 
	{
	public:
		GouraudShader(const std::vector<Light*>& _lights, Camera*& _camera) : Shader(_lights, _camera) { }
		virtual ~GouraudShader() { }
		virtual Pixel ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo);
	};

	class PhongShader : public Shader 
	{
	public:
		PhongShader(const std::vector<Light*>& _lights, Camera*& _camera) : Shader(_lights, _camera) { }
		virtual ~PhongShader() { }
		virtual Pixel ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo);
	};
}

#endif // !_SHADERS_H
