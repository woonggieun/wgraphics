#if defined(DEBUG) | defined (_DEBUG)
#define CRTDEBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h>
#include "Debug.h"
#include "Application.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int show)
{
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	WE::Application::InitData initData;

	initData.instance     = instance;
	initData.isFullscreen = false;
	initData.width        = 1280;
	initData.height       = 720;
	initData.title        = "WEngine";

	WE::Application::Application app(initData);
	app.Update();

	return 0;
}