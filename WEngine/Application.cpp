#include "Application.h"
#include <iostream>

namespace
{
	// The name of window class
	const char* CLASS_NAME = "W Engine";
	// Window style if the user choose fullscreen mode
	DWORD FULLSCREEN_STYLE = WS_POPUP | WS_VISIBLE;
	// Window style if the user choose window mode
	DWORD WINDOWED_STYLE = WS_POPUP | WS_CAPTION;

	void AdjustAndCenterWindow(DWORD style, RECT& size, int& xStart, int& yStart)
	{
		DEVMODE dm = { 0 }; 
		int winWidth, winHeight;

		// Get the size of the screen
		dm.dmSize = sizeof(dm);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

		AdjustWindowRect(&size, style, false);

		winWidth = size.right - size.left;
		winHeight = size.bottom - size.top;

		xStart = (dm.dmPelsWidth / 2) - (winWidth / 2);
		yStart = (dm.dmPelsHeight / 2) - (winHeight / 2);
	}
}

namespace WE
{
	namespace Application
	{
		Application::Application(const InitData& initData)
		{
			DEBUG_CALL_CHECK();

			m_instance = initData.instance;
			m_width = initData.width;
			m_height = initData.height;
			m_isFullScreen = initData.isFullscreen;
			m_style = (initData.isFullscreen) ? FULLSCREEN_STYLE : WINDOWED_STYLE;
			m_isQuitting = false;

			m_winClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
			m_winClass.cbClsExtra = 0;
			m_winClass.cbWndExtra = 0;
			m_winClass.lpszMenuName = 0;
			m_winClass.hIcon = LoadIcon(0, IDI_APPLICATION);
			m_winClass.hCursor = LoadCursor(0, IDC_ARROW);
			m_winClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			m_winClass.hInstance = initData.instance;
			m_winClass.lpszClassName = CLASS_NAME;
			m_winClass.lpfnWndProc = WinProc;

			RegisterClass(&m_winClass);

			int xStart, yStart;
			RECT size = { 0 };
			size.right = initData.width;
			size.bottom = initData.height;
			AdjustAndCenterWindow(m_style, size, xStart, yStart);

			m_window = CreateWindow(
				CLASS_NAME,      // Class name
				initData.title,  // Window name
				m_style,         // Window style
				xStart,          // x starting pos
				yStart,          // y starting pos
				size.right - size.left, // Width
				size.bottom - size.top, // Height
				0,                      // Parent Window 
				0,                      // Menu
				m_instance,             // hInstance
				this                    // lParam, this will be available in WM_CREATE
				);

			ShowWindow(m_window, true);
			UpdateWindow(m_window);
		}

		Application::~Application(void)
		{
			DEBUG_CALL_CHECK();

			UnregisterClass(CLASS_NAME, m_instance);
			m_instance = 0;
		}

		void Application::Update(void)
		{
			DEBUG_CALL_CHECK();

			while (!m_isQuitting)
			{
				ProcessMessage();
				// Update GameManager

				//If GameManager is Quitting, set close message.	
			}
		}

		void Application::ProcessMessage(void)
		{
			MSG msg;

			while (PeekMessage(&msg, m_window, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		void Application::Quit(void)
		{
			SendMessage(m_window, WM_CLOSE, 0, 0);
		}

		int Application::GetWidth(void) const
		{
			return m_width;
		}

		int Application::GetHeight(void) const
		{
			return m_height;
		}

		LRESULT CALLBACK Application::WinProc(HWND win, UINT msg, WPARAM wp, LPARAM lp)
		{
			static Application* s_pApp = 0;
			switch (msg)
			{
			case WM_CREATE:
			{
				CREATESTRUCT* pCS = reinterpret_cast<CREATESTRUCT*>(lp);
				s_pApp = reinterpret_cast <Application*>(pCS->lpCreateParams);

				//Create graphics
				break;
			}
			case WM_DESTROY:
			{
				s_pApp->m_isQuitting = true;
				s_pApp->m_window = 0;
				PostQuitMessage(0);
				break;
			}
			case WM_CLOSE:
			{
				//First destroy the graphics
				//Then destroy the window
				DestroyWindow(win);
				break;
			}
			default:
				return DefWindowProc(win, msg, wp, lp);
			}

			return 0;
		}
	}
}