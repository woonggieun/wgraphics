#ifndef APP_H
#define APP_H

#include <windows.h>
#include "Debug.h"

namespace WE
{
	namespace Application
	{
		struct InitData
		{
			const char*  title;         // Title of the window
			HINSTANCE    instance;      // The instance from main
			bool         isFullscreen;  // Boolean for checking fullscreen
			int			 width;         // The width of window
			int          height;        // The height of window
		};

		class Application
		{
		public:
			Application(const InitData& initData);
			~Application(void);

			// Application functions called by main
			void Update(void);

			// Function called in main game loop
			void ProcessMessage(void);

			// Functions to control the window
			void Quit(void);
			void SetFullScreen(bool fullScreen);
			void SetShowWindow(bool show);
			void SetShowCursor(bool show);
			void ChangeResolution(int width, int height);
			int  GetWidth(void) const;
			int  GetHeight(void)const;

		private:
			static LRESULT CALLBACK WinProc(HWND win, UINT msg, WPARAM wParam, LPARAM lParam);

			HINSTANCE m_instance;
			WNDCLASS  m_winClass;
			DWORD     m_style;
			HWND      m_window;
			bool      m_isQuitting;
			bool      m_isFullScreen;
			int       m_width;
			int       m_height;
		};
	}
}

#endif