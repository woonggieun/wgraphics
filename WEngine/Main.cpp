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

	WE::Application::FrameWork frameWork;

	if (frameWork.Initialize())
	{
		frameWork.Run();
	}
		
	return 0;
}