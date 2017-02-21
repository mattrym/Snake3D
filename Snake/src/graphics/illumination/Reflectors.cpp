#include "graphics\illumination\Reflectors.h"

namespace gfx 
{
	glm::vec3 PhongReflector::Reflect(const Vertex& vertex, const MaterialInfo& mInfo)
	{
		glm::vec3 color = mInfo.Ambient();
		std::vector<Light*>::const_iterator light_it;

		for (light_it = lights.cbegin(); light_it != lights.cend(); ++light_it)
		{
			Light* light = *light_it;
			glm::vec4 V = camera->ViewVector(vertex), N = vertex.Normal();
			glm::vec4 L = light->LightVector(vertex), R = light->ReflectedVector(vertex);

			float D = std::fmaxf(glm::dot(N, L), 0);
			float S = std::powf(std::fmaxf(glm::dot(V, R), 0), mInfo.N());
			color += D * mInfo.Diffuse() * light->LightColor(vertex);
			color += S * mInfo.Specular() * light->LightColor(vertex);
		}

		for (int i = 0; i < 3; ++i) {
			color[i] = glm::clamp(color[i], 0.f, 1.f);
		}
		return color;
	}

	glm::vec3 BlinnReflector::Reflect(const Vertex& vertex, const MaterialInfo& mInfo)
	{
		glm::vec3 color = mInfo.Ambient();
		std::vector<Light*>::const_iterator light_it;

		for (light_it = lights.cbegin(); light_it != lights.cend(); ++light_it)
		{
			Light* light = *light_it;
			glm::vec4 V = camera->ViewVector(vertex), N = vertex.Normal(), L = light->LightVector(vertex);
			glm::vec4 H = glm::normalize(L + V);

			float D = std::fmaxf(glm::dot(N, L), 0);
			float S = std::powf(std::fmaxf(glm::dot(N, H), 0), mInfo.N());
			color += D * mInfo.Diffuse() * light->LightColor(vertex);
			color += S * mInfo.Specular() * light->LightColor(vertex);
		}

		for (int i = 0; i < 3; ++i) {
			color[i] = glm::clamp(color[i], 0.f, 1.f);
		}
		return color;
	}
}