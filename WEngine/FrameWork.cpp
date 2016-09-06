#include "FrameWork.h"
#include "SystemDef.h"

#include <iostream>

namespace WE
{
	namespace Application
	{
		FrameWork::FrameWork(void)
		{

		}

		FrameWork::~FrameWork(void)
		{
			if (FULL_SCREEN)
			{
				ChangeDisplaySettings(NULL, 0);
			}

			Engine::GetEngine()->Release();

			UnregisterClass(m_applicationName, m_hInstance);
			m_hInstance = NULL;
		}

		bool FrameWork::Initialize(void)
		{
			if (!CreateDXWindow("DXEngine", 
				WINDOW_POSX, WINDOW_POSY, 
				SCREEN_WIDTH, SCREEN_HEIGHT))
			{
				return false;
			}

			if (!Engine::GetEngine()->
				Initialize(m_hInstance, 
				Engine::GetEngine()->GetGraphics()->GetHWND()))
			{
				return false;
			}

			return true;
		}

		void FrameWork::Run(void)
		{
			MSG msg;
			ZeroMemory(&msg, sizeof(MSG));

			//update loop
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					//update & render functions
					Engine::GetEngine()->Run();
				}
			}

		}
		
		bool FrameWork::CreateDXWindow(char* winTitle, int x, int y, int width, int height)
		{
			HWND hwnd;
			WNDCLASSEX wc;

			m_applicationName = winTitle;

			m_hInstance = GetModuleHandle(NULL);

			//Setup the windows class with 'DEFAULT' settings
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.lpfnWndProc   = WinProc;
			wc.cbClsExtra    = 0;
			wc.cbWndExtra    = 0;
			wc.hInstance     = m_hInstance;
			wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIconSm       = wc.hIcon;
			wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.lpszMenuName  = NULL;
			wc.lpszClassName = m_applicationName;
			wc.cbSize        = sizeof(WNDCLASSEX);

			if (!RegisterClassEx(&wc))
			{
				MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
				return false;
			}

			//TODO: FULL_SCREEN
			int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);

			if (FULL_SCREEN)
			{
				DEVMODE dmScreenSettings;
				memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
				dmScreenSettings.dmSize = sizeof(dmScreenSettings);
				dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
				dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
				dmScreenSettings.dmBitsPerPel = 32;
				dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

				ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
			}
			else
			{
				screenWidth  = width;
				screenHeight = height;
			}

			int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;


			hwnd = CreateWindowEx(WS_EX_APPWINDOW, winTitle, winTitle,nStyle, x, y, width, height,
				NULL, NULL, m_hInstance, NULL);

			if (hwnd == NULL)
			{
				MessageBox(NULL, "CreateWindowEX() failed", "Error", MB_OK);
				Engine::GetEngine()->Release();
				PostQuitMessage(0);

				return false;
			}

			if (!Engine::GetEngine()->InitializeGraphics(hwnd))
			{
				MessageBox(hwnd, "Could not initialize DirectX 11", "Error", MB_OK);
				Engine::GetEngine()->Release();
				PostQuitMessage(0);
				UnregisterClass(m_applicationName, m_hInstance);
				m_hInstance = NULL;
				DestroyWindow(hwnd);

				return false;
			}

			Engine::GetEngine()->GetGraphics()->SetHWND(hwnd);
			
			ShowWindow(hwnd, SW_SHOW);
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);

			return true;
		}

		LRESULT CALLBACK FrameWork::WinProc(HWND win, UINT msg, WPARAM wp, LPARAM lp)
		{
			PAINTSTRUCT ps;
			HDC hdc;

			switch (msg)
			{
			case WM_PAINT:
				{
					hdc = BeginPaint(win, &ps);
					EndPaint(win, &ps);
				}break;
			case WM_CLOSE:
				{
					PostQuitMessage(0);
					DestroyWindow(win);
			    }break;
			default:
				{
					return DefWindowProc(win, msg, wp, lp);
				}

			}

			return 0;
		}
	}
}