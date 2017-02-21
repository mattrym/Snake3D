#include "graphics\illumination\Shaders.h"

namespace gfx 
{
	glm::vec3 interpolate(int x, int y, Pixel* pnt[3]) 
	{
		glm::vec3 out;
		glm::vec3 p[3], _p;
		float d[3], _d;

		_p = glm::vec3(x, y, 1);
		for (int i = 0; i < 3; ++i) {
			p[i] = glm::vec3(pnt[i]->X(), pnt[i]->Y(), 1);
		}

		if (!(_d = glm::determinant(glm::mat3(p[0], p[1], p[2])))) {
			return glm::vec3(1 / 3.f, 1 / 3.f, 1 / 3.f);
		}

		d[0] = glm::determinant(glm::mat3(_p, p[1], p[2]));
		d[1] = glm::determinant(glm::mat3(p[0], _p, p[2]));
		d[2] = glm::determinant(glm::mat3(p[0], p[1], _p));

		_d = d[0] + d[1] + d[2];

		for (int i = 0; i < 3; ++i) {
			out[i] = d[i] / _d;
		}
		return out;
	}

	Pixel PlainShader::ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo)
	{
		int z;
		float zFloat = 0;
		glm::vec3 color;
		glm::vec4 point, normal;
		glm::vec3 cofactors = interpolate(x, y, pnt);
		
		for (int i = 0; i < 3; ++i) {
			zFloat += cofactors[i] * pnt[i]->Z();
			color += pnt[i]->Color();
			point += cofactors[i] * pnt[i]->WorldVertex().Point();
			normal += cofactors[i] * pnt[i]->WorldVertex().Normal();
		}

		z = (int)zFloat;
		color /= 3;
		point /= point.w;
		glm::normalize(normal);

		return Pixel(x, y, z, color, Vertex(point, normal));
	}

	Pixel GouraudShader::ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo)
	{
		int z;
		float zFloat = 0;
		glm::vec3 color;
		glm::vec4 point, normal;
		glm::vec3 cofactors = interpolate(x, y, pnt);
		
		for (int i = 0; i < 3; ++i) {
			zFloat += cofactors[i] * pnt[i]->Z();
			color += cofactors[i] * pnt[i]->Color();
			point += cofactors[i] * pnt[i]->WorldVertex().Point();
			normal += cofactors[i] * pnt[i]->WorldVertex().Normal();
		}

		z = (int)zFloat;
		for (int i = 0; i < 3; ++i) {
			color[i] = glm::clamp(color[i], 0.f, 1.f);
		}
		
		return Pixel(x, y, z, color, Vertex(point, normal));
	}

	Pixel PhongShader::ShadePixel(int x, int y, Pixel* pnt[3], const MaterialInfo& mInfo)
	{
		int z;
		float zFloat = 0;
		glm::vec3 color;
		glm::vec4 point, normal;
		glm::vec3 cofactors = interpolate(x, y, pnt);
		
		for (int i = 0; i < 3; ++i) {
			zFloat += cofactors[i] * pnt[i]->Z();
			point += cofactors[i] * pnt[i]->WorldVertex().Point();
			normal += cofactors[i] * pnt[i]->WorldVertex().Normal();
		}

		z = (int)zFloat;
		point /= point.w;
		glm::normalize(normal);

		color = mInfo.Ambient();
		Vertex vertex(point, normal);
		std::vector<Light*>::const_iterator light_it;

		for (light_it = lights.cbegin(); light_it != lights.cend(); ++light_it)
		{
			Light* light = *light_it;
			glm::vec4 V = camera->ViewVector(vertex), N = vertex.Normal();
			glm::vec4 L = light->LightVector(vertex), R = light->ReflectedVector(vertex);

			float D = glm::clamp(glm::dot(N, L), 0.f, 1.f);
			float S = std::powf(glm::clamp(glm::dot(V, R), 0.f, 1.f), mInfo.N());
			color += D * mInfo.Diffuse() * light->LightColor(vertex);
			color += S * mInfo.Specular() * light->LightColor(vertex);
		}

		for (int i = 0; i < 3; ++i) {
			color[i] = glm::clamp(color[i], 0.f, 1.f);
		}

		return Pixel(x, y, z, color, vertex);
	}
}