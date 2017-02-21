#pragma once

#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>
#include "Graphics.h"
#include "Snake.h"

namespace game
{
	class Animation
	{
	private:
		static glm::vec4 trackVectors[4];

		const float UNIT;
		const int BOARD_WIDTH;
		const int BOARD_HEIGHT;
		const int SMOOTHNESS;

		Snake* snake;
		gfx::Renderer* renderer;
		std::vector<gfx::Model*> models;
		std::vector<gfx::Light*> lights;

	public:
		Animation(gfx::Renderer* _renderer, int _board_width, int _board_height, float _unit, int _smoothness);
		virtual ~Animation();

		virtual bool Initialize();
		void Animate();
		DIR ValidateDirection(DIR direction);

		void ChangeDirection(DIR dir);
		void ChangeCamera();
		void ChangeReflectionModel();
		void ChangeShadingModel();
	};
}

#endif // !_ANIMATION_H
