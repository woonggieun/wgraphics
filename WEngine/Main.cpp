#if defined(DEBUG) | defined (_DEBUG)
#define CRTDEBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h>
#include "Debug.h"
#include "FrameWork.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int show)
{
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	WE::Application::FrameWork* frameWork = new WE::Application::FrameWork();

	if (frameWork->Initialize())
	{
		frameWork->Run();
	}

	delete frameWork;
		
	return 0;
}