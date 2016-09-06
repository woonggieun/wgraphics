/*******************************************************************************

filename    Main.cpp

author      Woonggi Eun

email       dmsdndre@gmail.com

date        Sep.06.2016

Brief Description:
It containts main function to run game engine.

*******************************************************************************/

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