/*******************************************************************************

filename    Graphics.cpp

author      Woonggi Eun

email       dmsdndre@gmail.com

date        Sep.06.2016

Brief Description:
Graphics class which manages graphics part such as directX 11 device,
device context and so on.

*******************************************************************************/

#include "Graphics.h"

namespace WE
{
	namespace Graphics
	{
		Graphics::Graphics(void)
		{
			m_dxManager = NULL;
		}

		Graphics::~Graphics(void)
		{
			if (m_dxManager)
			{
				delete m_dxManager;
				m_dxManager = NULL;
			}
		}

		bool Graphics::InitializeDX(HWND hwnd)
		{
			m_dxManager = new DXManager();

			//If it failed to initialize dx manager, 
			if (!m_dxManager->Initialize((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT,
				VSYNC_ENABLED, hwnd, FULL_SCREEN))
			{
				return false;
			}

			return true;
		}

		void Graphics::Initialize(void)
		{

		}			
		
		void Graphics::BeginScene(float r, float g, float b, float a)
		{
			m_dxManager->BeginScene(r, g, b, a);
		}

		void Graphics::EndScene(void)
		{
			m_dxManager->EndScene();
		}

		void Graphics::EnableAlphaBlending(bool enable)
		{
			m_dxManager->EnableAlphaBlending(enable);
		}

		void Graphics::EnableZBuffer(bool enable)
		{
			m_dxManager->EnableZBuffer(enable);
		}
		
		//Gettors
		WE::Graphics::DXManager* Graphics::GetDXManager(void)
		{
			return m_dxManager;
		}

		HWND Graphics::GetHWND(void)
		{
			return m_hwnd;
		}

		ID3D11Device* Graphics::GetDevice(void)
		{
			return m_dxManager->GetDevice();
		}
		
		ID3D11DeviceContext* Graphics::GetDeviceContext(void)
		{
			return m_dxManager->GetDeviceContext();
		}
		
		void Graphics::SetHWND(HWND hwnd)
		{
			m_hwnd = hwnd;
		}

	}
}
