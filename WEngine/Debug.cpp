#include "Debug.h"
#include <sstream> //For stringstream class
#include <string>

namespace DebugTools
{
	bool Assert(int expression, const char* outputMessage,
		const char* functionName,
		const char* fileName,
		unsigned int lineNumber)
	{
		if (!expression)
		{
			int returnValue;
			std::stringstream ss;

			//Set Output Message
			ss << "Assertion Failure :";
			ss << "\nFile: ";
			ss << fileName;
			ss << "\nLine: ";
			ss << lineNumber;
			ss << "\nFunction: ";
			ss << functionName;
			ss << "\n\n Description: ";
			ss << outputMessage;
			ss << "\n\nYes : Break into the Debugger";
			ss << "\nNo : Exit immediately";

			//display a message to the user
			returnValue = MessageBox(NULL, ss.str().c_str(), "ASSERT!",
				MB_TASKMODAL | MB_SETFOREGROUND | MB_YESNO | MB_ICONERROR);

			if (returnValue == IDYES)
			{
				return true;
			}
			ExitProcess((unsigned)(-1));

		}
		return false;

	}

	void CreateConsole(void)
	{
		FILE* pFile;
		AllocConsole();

		freopen_s(&pFile, "CONOUT$", "wt", stdout);
		freopen_s(&pFile, "CONOUT$", "wt", stderr);

		SetConsoleTitle("Debug Console");
	}

	void DestroyConsole(void)
	{
		FreeConsole();
	}

	void ClearScreen(void)
	{
		//home for the cursor
		COORD coordScreen = { 0, 0 };
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD dwConSize;
		HANDLE hStdout;
		int result;

		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

		//Get the number of character cells in the current buffer.
		if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
			return;

		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		result = FillConsoleOutputCharacter(
			hStdout,         //Handle to console screen buffer
			' ',             //Character to write to the buffer
			dwConSize,       //Number of cells to write
			coordScreen,     //Coordinates of first cell
			&cCharsWritten); // Receive number of characters written

		if (!result)
			return;

		if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
			return;

		result = FillConsoleOutputAttribute(
			hStdout,          // Handle to console screen buffer
			csbi.wAttributes, // Character attributes to use
			dwConSize,        // Number of cells to set attribute
			coordScreen,      // Coordinates of first cell
			&cCharsWritten);  // Receive number of characters written

		if (!result)
			return;

		SetConsoleCursorPosition(hStdout, coordScreen);
	}

}