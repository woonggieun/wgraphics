#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "DXManager.h"
#include "SystemDef.h"

namespace WE
{
	namespace Graphics
	{
		class Graphics
		{
		public:
			Graphics(void);
			~Graphics(void);

			bool InitializeDX(HWND hwnd);

			void Initialize(void);
			void BeginScene(float r, float g, float b, float a);
			void EndScene(void);

			void EnableAlphaBlending(bool enable);
			void EnableZBuffer      (bool enable);

			//Gettors
			WE::Graphics::DXManager* GetDXManager(void);
			HWND GetHWND                         (void);
			ID3D11Device* GetDevice              (void);
			ID3D11DeviceContext* GetDeviceContext(void);

			void SetHWND(HWND hwnd);

		private:
			DXManager* m_dxManager;
			HWND       m_hwnd;
		};
	}
}

#endif