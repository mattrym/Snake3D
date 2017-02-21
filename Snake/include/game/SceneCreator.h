#pragma once

#ifndef _SCENECREATOR_H
#define _SCENECREATOR_H

#include <vector>
#include "Graphics.h"
#include "Snake.h"

namespace game
{
	class SceneCreator
	{
	private:
		const float UNIT;
		const int SMOOTHNESS;
		gfx::SolidFactory& factory;
		gfx::Renderer* renderer;

		Snake* snake;
		std::vector<gfx::Model*> models;
	
	public:
		SceneCreator(gfx::Renderer* _renderer, float unit, int smoothness);
		virtual ~SceneCreator();

		void CreateBoard(int board_width, int board_height, const gfx::MaterialInfo & mGroundInfo, const gfx::MaterialInfo & mWallInfo);
		void AppendSnakeSegment(const gfx::MaterialInfo& mInfo);
		void PlaceCube(float length, int smoothness, glm::ivec2 position, const gfx::MaterialInfo& mInfo);
		void PlaceCylinder(float length, int smoothness, glm::ivec2 position, const gfx::MaterialInfo& mInfo);
		void PlaceSphere(float length, int smoothness, glm::ivec2 position, const gfx::MaterialInfo& mInfo);
		void GetScene(Snake*& _snake, std::vector<gfx::Model*>& _models);
	};
}

#endif // !_SCENECREATOR_H
