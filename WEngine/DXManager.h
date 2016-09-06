/*******************************************************************************

filename    DXManager.h

author      Woonggi Eun

email       dmsdndre@gmail.com

date        Sep.06.2016

Brief Description:
Header file for implementing DXManager.

*******************************************************************************/

#ifndef DXMANAGER_H 
#define DXMANAGER_H

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

namespace WE
{
	namespace Graphics
	{
		class DXManager
		{
		public:
			DXManager(void);
			~DXManager(void);

			bool Initialize(int width, int height, bool vsync, HWND hwnd, bool fullScreen);
			void BeginScene(float r, float g, float b, float a);
			void EndScene(void);

			void EnableAlphaBlending(bool enable);
			void EnableZBuffer(bool enable);

			ID3D11Device* GetDevice(void);
			ID3D11DeviceContext* GetDeviceContext(void);

		private:
			// Initialize Functions
			bool InitializeSwapChain(HWND hwnd, bool fullScreen, int width, int height, unsigned numerator, unsigned denominator);
			bool InitializeDepthBuffer(int width, int height);
			bool InitializeDepthStencilBuffer(void);
			bool InitializeStencilView(void);
			bool InitializeRasterizerState(void);
			void InitializeViewport(int width, int height);
			bool InitializeAlphaBlending(void);
			bool InitializeZBuffer(void);

			bool m_vsync_enabled;
			int m_videoCardMemory;
			char m_videoCardDescription[128];
			IDXGISwapChain* m_swapChain;
			ID3D11Device* m_device;
			ID3D11DeviceContext* m_deviceContext;
			ID3D11RenderTargetView* m_renderTargetView;
			ID3D11Texture2D* m_depthStencilBuffer;
			//Stencil buffer contains per-pixel integer data which is used
			//to add more control over whichi pixels are rendered.
			ID3D11DepthStencilState* m_depthStencilState;
			ID3D11DepthStencilView* m_depthStencilView;
			ID3D11RasterizerState* m_rasterState;
			ID3D11BlendState* m_alphaEnableBlendingState;
			ID3D11BlendState* m_alphaDisableBlendingState;
			ID3D11DepthStencilState* m_depthDisabledStencilState;
		};
	}
}

#endif 