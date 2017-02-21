#include "application.h"

int main(int argc, char* args[])
{
	Application app(800, 600);
	app.onInit(20, 20, 6.0f, 20);
	return app.onExecute();
}