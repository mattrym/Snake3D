#pragma once

#ifndef _RENDERER_H
#define _RENDERER_H

#include <SDL2\SDL.h>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

#include "graphics\Camera.h"
#include "graphics\models\Model.h"
#include "graphics\structures\Mesh.h"
#include "graphics\structures\MaterialInfo.h"
#include "graphics\illumination\Lights.h"
#include "graphics\illumination\Shaders.h"
#include "graphics\illumination\Reflectors.h"

namespace gfx 
{
	enum CAMERA 
	{
		FIXED_CAMERA,
		INTERACTIVE_CAMERA,
		TRACKING_CAMERA
	};
	enum REFLECTOR 
	{
		PHONG_REFLECTOR,
		BLINN_REFLECTOR
	};
	enum SHADER 
	{
		PLAIN_SHADER,
		GOURAUD_SHADER,
		PHONG_SHADER
	};

	class Renderer 
	{
	private:
		const int SCREEN_WIDTH;
		const int SCREEN_HEIGHT;

		int** zBuffer;
		SDL_Renderer* sdlrenderer;
		glm::mat4x4 projectionMatrix;
		
		std::vector<Model*> models;
		std::vector<Light*> lights;

		glm::vec4 cameraStaticPosition;
		glm::vec4 cameraStaticTarget;
		glm::vec4 cameraTrackingDistance;
		Model* cameraTrackedObject;

		CAMERA currentCamera;
		REFLECTOR currentReflector;
		SHADER currentShader;
		Camera* camera;
		Reflector* reflector;
		Shader* shader;
		
		static glm::mat4x4 CreatePerspectiveFieldOfView(float theta, float a, float n, float f);
		bool CheckVisibility(glm::vec4 projectionVertices[3]);
		void FillTriangle(Pixel* pnt[3], const MaterialInfo& mInfo);
		void DrawLine(float x1, float x2, int y, Pixel* pnt[3], const MaterialInfo& m);
		Pixel* ProjectVertex(const Vertex & wVertex, glm::vec4 pVertex, const MaterialInfo& mInfo);

	public:
		Renderer(SDL_Renderer* _sdlrenderer, int screenWidth, int screenHeight);
		virtual ~Renderer();

		void AddLight(Light* light);
		void AddModel(Model* model);

		void SetCameraPosition(glm::vec4& _cameraStaticPosition);
		void SetCameraStaticTarget(glm::vec4& _cameraStaticTarget);
		void SetCameraTrackedObject(Model* _cameraTrackedObject);
		void SetCameraTrackingDistance(glm::vec4& _cameraTrackingDistance);

		void SetCamera(CAMERA _camera);
		void SetShader(SHADER _shader);
		void SetReflector(REFLECTOR _illuminator);		
		
		CAMERA CurrentCamera() const { return currentCamera; }
		REFLECTOR CurrentReflector() const { return currentReflector; }
		SHADER CurrentShader() const { return currentShader; }

		void RenderScene();
	};
}

#endif // !_RENDERER_H
