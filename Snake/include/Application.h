#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <vector>
#include <SDL2\SDL.h>
#include "graphics.h"
#include "game\Animation.h"

class Application 
{
private:
	const int WIDTH, HEIGHT;
	bool initialized, running, animating;

	SDL_Window* window;
	SDL_Renderer* SDLrenderer;
	gfx::Renderer* renderer3d;
	game::Animation* animation;

public:
	Application(int width, int height);
	virtual ~Application();

	bool onInit(int _board_width, int _board_height, float _unit, int _smoothness);
	int onExecute();
	void onEvent(SDL_Event* event);
	void onLoop();
	void onRender();
	void onCleanup();
};

#endif // !_APPLICATION_H

