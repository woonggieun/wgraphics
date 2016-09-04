#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <Windows.h>

namespace WE
{
	namespace Application
	{
		class FrameWork
		{
		public:
			FrameWork(void);
			~FrameWork(void);

			bool Initialize(void);
			void Run(void);

		private:
			static LRESULT CALLBACK WinProc(HWND win, UINT msg, WPARAM wParam, LPARAM lParam);

			bool CreateDXWindow(char* windowTitle, int x, int y, int width, int height);

			char* m_applicationName;
			HINSTANCE m_hInstance;
		};
	}
}
#endif