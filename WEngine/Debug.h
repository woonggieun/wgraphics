/*******************************************************************************

filename    Debug.h 

author      Woonggi Eun

email       dmsdndre@gmail.com

date        Sep.06.2016

Brief Description:
Header file for implementing debugging functions.

*******************************************************************************/

#define WIN32_LEAN_AND_MEAN

#if defined(DEBUG) | defined(_DEBUG)
#define CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Windows.h> //messagebox, debugbreak
#include <cstdio>	//printf 

namespace DebugTools
{
	bool Assert(int expression, const char* outputMessage,
		const char* functionName, const char* fileName,
		unsigned int lineNumber);

	void CreateConsole(void);
	void DestroyConsole(void);
	void ClearScreen(void);

	void WGMessageBox(const char* outputMessage);
}

#if defined(DEBUG) | defined(_DEBUG)

#define DEBUG_ASSERT(exp, str) if(DebugTools::Assert((exp),(str), \
__FUNCTION__, __FILE__, __LINE__))                           \
{                                                            \
	DebugBreak();                                            \
}

#define DEBUG_CREATE_CONSOLE() DebugTools::CreateConsole()
#define DEBUG_DESTROY_CONSOLE() DebugTools::DestroyConsole()
#define DEBUG_PRINT(...) printf(_VA_ARGS_)
#define DEBUG_CLEAR() DebugTools::ClearScreen()
//If you have a leak, there is a number in curly braces next to the error.
//Put that number in this function and check the call stack to see when 
//and where the allocation happened. Set it to -1 to have it not break.
#define DEBUG_LEAK_CHECKS(x) \
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG); \
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); \
	_CrtSetBreakAlloc((x));

#define DEBUG_CALL_CHECK() \
	static int functionCounter = 0;\
	++functionCounter; \
	DEBUG_ASSERT(functionCounter == 1, "This function should be called only once!");


#else

#define DEBUG_ASSERT(expression, outputMessage)
#define DEBUG_CREATE_CONSOLE() 
#define DEBUG_DESTROY_CONSOLE()
#define DEBUG_PRINT(...)
#define DEBUG_CLEAR()
#define DEBUG_LEAK_CHECKS(x)

#endif