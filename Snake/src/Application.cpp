#include <iostream>
#include "application.h"

Application::Application(int width, int height) : WIDTH(width), HEIGHT(height)
{
	initialized = false;
	running = animating = true;

	window = nullptr;
	SDLrenderer = nullptr;
	renderer3d = nullptr;
	animation = nullptr;
}

Application::~Application()
{
	delete renderer3d;
	delete animation;
}

bool Application::onInit(int _board_width, int _board_height, float _unit, int _smoothness)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		return false;
	}
	if (!(window = SDL_CreateWindow("Snake3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN))) 
	{
		std::cerr << "Creating window ended in failure: " << SDL_GetError() << std::endl;
		return false;
	}
	if (!(SDLrenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
	{
		std::cerr << "Creating SDL renderer ended in failure: " << SDL_GetError() << std::endl;
		return false;
	}
	if (!(renderer3d = new gfx::Renderer(SDLrenderer, WIDTH, HEIGHT))) 
	{
		std::cerr << "Creating scene renderer ended in failure: " << SDL_GetError() << std::endl;
		return false;
	}
	if (!(animation = new game::Animation(renderer3d, _board_width, _board_height, _unit, _smoothness)))
	{
		std::cerr << "Creating animation ended in failure: " << SDL_GetError() << std::endl;
		return false;
	}

	initialized = animation->Initialize();
	return initialized;
}

int Application::onExecute()
{
	if (initialized == false) 
	{
		return -1;
	}

	SDL_Event event;
	
	while (running) 
	{
		while (SDL_PollEvent(&event)) 
		{
			onEvent(&event);
		}
		onLoop();
		onRender();
	}
	onCleanup();

	return 0;
}

void Application::onEvent(SDL_Event * event)
{
	switch (event->type)
	{
	case SDL_QUIT:
		running = false;
		break;
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) 
		{
		case SDLK_SPACE:
			animating = !animating;
			break;
		case SDLK_c:
			animation->ChangeCamera();
			break;
		case SDLK_r:
			animation->ChangeReflectionModel();
			break;
		case SDLK_s:
			animation->ChangeShadingModel();
			break;
		case SDLK_UP:
			animation->ChangeDirection(game::NORTH);
			break;
		case SDLK_LEFT:
			animation->ChangeDirection(game::WEST);
			break;
		case SDLK_DOWN:
			animation->ChangeDirection(game::SOUTH);
			break;
		case SDLK_RIGHT:
			animation->ChangeDirection(game::EAST);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void Application::onLoop()
{
	if (animating)
	{
		animation->Animate();
		SDL_Delay(10);
	}
}

void Application::onRender()
{
	SDL_SetRenderDrawColor(SDLrenderer, 0, 0, 0, 255);
	SDL_RenderClear(SDLrenderer);
	renderer3d->RenderScene();
	SDL_RenderPresent(SDLrenderer);
}

void Application::onCleanup()
{
	SDL_DestroyRenderer(SDLrenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
