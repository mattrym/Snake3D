#include "game\SceneCreator.h"
#include "game\Animation.h"

namespace game
{
	glm::vec4 Animation::trackVectors[4] = 
	{
		glm::vec4(0.0f, 0.5f, 1.0f, 0.0f),
		glm::vec4(1.0f, 0.5f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.5f, -1.0f, 0.0f),
		glm::vec4(-1.0f, 0.5f, 0.0f, 0.0f)
	};


	Animation::Animation(gfx::Renderer* _renderer, int _board_width, int _board_height, float _unit, int _smoothness)
		: renderer(_renderer), BOARD_WIDTH(_board_width), BOARD_HEIGHT(_board_height), UNIT(_unit), SMOOTHNESS(_smoothness)
	{
		snake = nullptr;
	}

	Animation::~Animation()
	{
		std::vector<gfx::Model*>::iterator model_it;
		std::vector<gfx::Light*>::iterator light_it;
		for (model_it = models.begin(); model_it != models.end(); ++model_it)
		{
			delete *model_it;
		}
		for (light_it = lights.begin(); light_it != lights.end(); ++light_it)
		{
			delete *light_it;
		}
		delete snake;
	}

	bool Animation::Initialize()
	{
		glm::vec3 white(1.0f, 1.0f, 1.0f);
		glm::vec3 brick(0.50f, 0.0f, 0.0f);
		glm::vec3 brown(0.55f, 0.27f, 0.08f);
		glm::vec3 red(1.0f, 0.0f, 0.0f);
		glm::vec3 green(0.0f, 1.0f, 0.0f);
		glm::vec3 yellow(1.0f, 1.0f, 0.0f);
		glm::vec3 blue(0.0f, 0.0f, 1.0f);

		gfx::MaterialInfo mGround(11.0f, 0.1f * brown, brown, 0.5f * white);
		gfx::MaterialInfo mWall(0.5f, 0.1f * brick, brick, 0.7f*white);
		gfx::MaterialInfo mSnake(3.0f, 0.1f * green, green, white);

		gfx::MaterialInfo mRed(5.0f, 0.1f * red, red, 0.75f * white);
		gfx::MaterialInfo mGreen(5.0f, 0.1f * green, green, 0.75f * white);
		gfx::MaterialInfo mBlue(5.0f, 0.1f * blue, blue, 0.75f * white);
		gfx::MaterialInfo mYellow(5.0f, 0.1f * yellow, yellow, 0.75f * white);

		SceneCreator *creator = new SceneCreator(renderer, UNIT, SMOOTHNESS);
		creator->CreateBoard(BOARD_WIDTH, BOARD_HEIGHT, mGround, mWall);	
		for (int i = 0; i < 10; ++i)
		{
			creator->AppendSnakeSegment(mSnake);
		}
		creator->PlaceCube(2 * UNIT, SMOOTHNESS, glm::ivec2(BOARD_WIDTH / 4, - BOARD_HEIGHT / 4), mRed);
		creator->PlaceCylinder(2 * UNIT, SMOOTHNESS, glm::ivec2(-BOARD_WIDTH / 4, BOARD_HEIGHT / 4), mYellow);
		creator->PlaceSphere(2 * UNIT, SMOOTHNESS, glm::ivec2(0, 0), mBlue);

		creator->GetScene(snake, models);
		delete creator;

		lights.push_back(new gfx::PointLight(glm::vec4(2 * UNIT * (-BOARD_WIDTH + 1), 50 * UNIT, 1 * UNIT * (-BOARD_HEIGHT + 1), 1), glm::vec3(1, 1, 1)));
		lights.push_back(new gfx::DirectionalLight(glm::vec4(-1.0f, -1.2f, -1.44f, 0.0f), 0.4f*glm::vec3(1, 1, 1)));
		
		for (std::vector<gfx::Light*>::iterator light_it = lights.begin(); light_it != lights.end(); ++light_it)
		{
			renderer->AddLight(*light_it);
		}

		renderer->SetCameraPosition(glm::vec4(1.2f * BOARD_WIDTH * UNIT, 1.44f * BOARD_WIDTH * UNIT, 1.73f * BOARD_HEIGHT * UNIT / 2, 1.0f));
		renderer->SetCameraStaticTarget(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		renderer->SetCameraTrackedObject(snake->HeadModel());

		for (int i = 0; i < 4; ++i)
		{
			trackVectors[i] *= 5.0f * UNIT;
		}
		renderer->SetCameraTrackingDistance(trackVectors[NORTH]);

		renderer->SetCamera(gfx::FIXED_CAMERA);
		renderer->SetReflector(gfx::PHONG_REFLECTOR);
		renderer->SetShader(gfx::PLAIN_SHADER);

		return snake && !models.empty();
	}

	void Animation::Animate()
	{
		DIR originalDirection = snake->Direction();
		DIR validatedDirection = ValidateDirection(originalDirection);
		if (originalDirection != validatedDirection)
		{
			snake->ChangeDirection(validatedDirection);
			renderer->SetCameraTrackingDistance(trackVectors[snake->Direction()]);
		}
		snake->MoveForward();
	}

	DIR Animation::ValidateDirection(DIR direction)
	{
		static float borders[4] =
		{
			UNIT * (BOARD_HEIGHT) / 2 - UNIT,
			UNIT * (-BOARD_WIDTH) / 2 + UNIT / 2.f,
			UNIT * (-BOARD_HEIGHT) / 2,
			UNIT * (BOARD_WIDTH) / 2 - UNIT / 2.f
		};

		int x = snake->Position().x;
		int y = snake->Position().y;

		if (y >= borders[NORTH] && direction == NORTH)
		{
			return EAST;
		}
		else if (x <= borders[WEST] && direction == WEST)
		{
			return NORTH;
		}
		else if (y <= borders[SOUTH] && direction == SOUTH)
		{
			return WEST;
		}
		else if (x >= borders[EAST] && direction == EAST)
		{
			return SOUTH;
		}
		return direction;
	}

	void Animation::ChangeCamera()
	{
		switch (renderer->CurrentCamera())
		{
		case gfx::FIXED_CAMERA:
			renderer->SetCamera(gfx::INTERACTIVE_CAMERA);
			break;
		case gfx::INTERACTIVE_CAMERA:
			renderer->SetCamera(gfx::TRACKING_CAMERA);
			break;
		case gfx::TRACKING_CAMERA:
			renderer->SetCamera(gfx::FIXED_CAMERA);
			break;
		default:
			break;
		}
	}

	void Animation::ChangeDirection(DIR direction)
	{
		if ((snake->Direction() - direction) % 2)
		{
			DIR originalDirection = snake->Direction();
			DIR validatedDirection = ValidateDirection(direction);
			if (originalDirection != validatedDirection)
			{
				snake->ChangeDirection(validatedDirection);
				renderer->SetCameraTrackingDistance(trackVectors[snake->Direction()]);
			}
		}
	}

	void Animation::ChangeReflectionModel()
	{
		switch (renderer->CurrentReflector())
		{
		case gfx::PHONG_REFLECTOR:
			renderer->SetReflector(gfx::BLINN_REFLECTOR);
			break;
		case gfx::BLINN_REFLECTOR:
			renderer->SetReflector(gfx::PHONG_REFLECTOR);
			break;
		default:
			break;
		}
	}

	void Animation::ChangeShadingModel()
	{
		switch (renderer->CurrentShader())
		{
		case gfx::PLAIN_SHADER:
			renderer->SetShader(gfx::GOURAUD_SHADER);
			break;
		case gfx::GOURAUD_SHADER:
			renderer->SetShader(gfx::PHONG_SHADER);
			break;
		case gfx::PHONG_SHADER:
			renderer->SetShader(gfx::PLAIN_SHADER);
			break;
		default:
			break;
		}
	}
}