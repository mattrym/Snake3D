#include <glm\gtc\random.hpp>
#include "game\SceneCreator.h"

namespace game 
{
	SceneCreator::SceneCreator(gfx::Renderer* _renderer, float unit, int smoothness)
		: renderer(_renderer), factory(gfx::SolidFactory::Instance()), UNIT(unit), SMOOTHNESS(smoothness)
	{
		snake = new Snake();
	}

	SceneCreator::~SceneCreator()
	{
		models.clear();
	}

	void SceneCreator::CreateBoard(int board_width, int board_height, const gfx::MaterialInfo& mGroundInfo, const gfx::MaterialInfo& mWallInfo)
	{
		// floor
		gfx::Model* floor = factory.CreateSurface(UNIT, board_width, board_height, mGroundInfo);
		floor->Translate(-UNIT * board_width / 2, 0, -UNIT * board_height / 2);
		gfx::Model* walls[4];

		// northern wall
		walls[NORTH] = factory.CreateSurface(UNIT, board_width, 1, mWallInfo);
		walls[NORTH]->RotateX(90);
		walls[NORTH]->Translate(-board_width * UNIT / 2, UNIT, -board_height * UNIT / 2);

		// western wall
		walls[WEST] = factory.CreateSurface(UNIT, 1, board_height, mWallInfo);
		walls[WEST]->RotateZ(-90);
	    walls[WEST]->Translate(-board_width * UNIT / 2, UNIT, -board_height * UNIT / 2);

		// southern wall
		walls[SOUTH] = factory.CreateSurface(UNIT, board_width, 1, mWallInfo);
		walls[SOUTH]->RotateX(-90);
		walls[SOUTH]->Translate(-board_width * UNIT / 2, 0, board_height * UNIT / 2);

		// eastern wall
		walls[EAST] = factory.CreateSurface(UNIT, 1, board_height, mWallInfo);
		walls[EAST]->RotateZ(90);
		walls[EAST]->Translate(board_width * UNIT / 2, 0, -board_height * UNIT / 2);

		models.push_back(floor);
		renderer->AddModel(floor);
		for (int i = 0; i < 4; ++i) 
		{
			models.push_back(walls[i]);
			renderer->AddModel(walls[i]);
		}
	}

	void SceneCreator::AppendSnakeSegment(const gfx::MaterialInfo& mInfo)
	{
		if (snake)
		{
			gfx::Model* model = factory.CreateSphere(UNIT / 2, SMOOTHNESS, SMOOTHNESS, mInfo);
			model->Translate(0, UNIT / 2, snake->length * UNIT - UNIT / 2);

			Snake::Segment segment(snake->length, glm::vec2(0, -snake->length * UNIT), model);
			segment.direction = NORTH;
			++snake->length;

			models.push_back(model);
			renderer->AddModel(model);
			snake->segments.push_back(segment);
		}
	}

	void SceneCreator::PlaceCube(float length, int smoothness, glm::ivec2 position, const gfx::MaterialInfo& mInfo)
	{
		gfx::Model* model = factory.CreateCuboid(length, length, length, mInfo);
		model->Translate(-length / 2, 0, -length / 2);
		model->Translate(UNIT * position.x, 0, UNIT * position.y);
		
		renderer->AddModel(model);
		models.push_back(model);
	}

	void SceneCreator::PlaceCylinder(float length, int smoothness, glm::ivec2 position, const gfx::MaterialInfo& mInfo)
	{
		gfx::Model* model = factory.CreateCylinder(length / 2, length, smoothness, mInfo);
		model->Translate(UNIT * position.x, 0, UNIT * position.y);

		renderer->AddModel(model);
		models.push_back(model);
	}

	void SceneCreator::PlaceSphere(float length, int smoothness, glm::ivec2 position, const gfx::MaterialInfo& mInfo)
	{
		gfx::Model* model = factory.CreateSphere(length / 2, smoothness, smoothness, mInfo);
		model->Translate(0, length / 2, 0);
		model->Translate(UNIT * position.x, 0, UNIT * position.y);

		renderer->AddModel(model);
		models.push_back(model);
	}

	void SceneCreator::GetScene(Snake *& _snake, std::vector<gfx::Model*>& _models)
	{
		if (!models.empty())
		{
			_snake = snake;
			_models = models;
		}
	}

}