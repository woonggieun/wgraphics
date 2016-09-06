#include "DXManager.h"

namespace WE
{
    namespace Graphics
    {
        DXManager::DXManager(void)
        {
            m_swapChain                  = 0;
            m_device                     = 0;
            m_deviceContext              = 0;
            m_renderTargetView           = 0;
            m_depthStencilBuffer         = 0;
            m_depthStencilState          = 0;
            m_depthStencilView           = 0;
            m_rasterState                = 0;
            m_alphaEnableBlendingState   = 0;
            m_alphaDisableBlendingState  = 0;
            m_depthDisabledStencilState  = 0;
        }

        DXManager::~DXManager(void)
        {
            // Before shutting down, set to windowed mode or when you release
            // the swap chain it will throw an exception.
            if (m_swapChain)
            {
                m_swapChain->SetFullscreenState(false, NULL);
            }

            if (m_alphaEnableBlendingState)
            {
                m_alphaEnableBlendingState->Release();
                m_alphaEnableBlendingState = 0;
            }

            if (m_alphaDisableBlendingState)
            {
                m_alphaDisableBlendingState->Release();
                m_alphaDisableBlendingState = 0;
            }

            if (m_depthDisabledStencilState)
            {
                m_depthDisabledStencilState->Release();
                m_depthDisabledStencilState = 0;
            }

            if (m_rasterState)
            {
                m_rasterState->Release();
                m_rasterState = 0;
            }

            if (m_depthStencilView)
            {
                m_depthStencilView->Release();
                m_depthStencilView = 0;
            }

            if (m_depthStencilState)
            {
                m_depthStencilState->Release();
                m_depthStencilState = 0;
            }

            if (m_depthStencilBuffer)
            {
                m_depthStencilBuffer->Release();
                m_depthStencilBuffer = 0;
            }

            if (m_renderTargetView)
            {
                m_renderTargetView->Release();
                m_renderTargetView = 0;
            }

            if (m_deviceContext)
            {
                m_deviceContext->Release();
                m_deviceContext = 0;
            }

            if (m_device)
            {
                m_device->Release();
                m_device = 0;
            }

            if (m_swapChain)
            {
                m_swapChain->Release();
                m_swapChain = 0;
            }
        }

        bool DXManager::Initialize(int width, int height, bool vsync, HWND hwnd, bool fullScreen)
        {
            HRESULT result;

            IDXGIFactory* factory;
            IDXGIAdapter* adapter;
            IDXGIOutput* adapterOutput;
            unsigned int numModes, numerator = 0, denominator = 0, strLen;
            DXGI_MODE_DESC* displayModeList;
            DXGI_ADAPTER_DESC adapterDesc;
            int error;
            ID3D11Texture2D* backBufferPtr;

            // store vsync setting
            m_vsync_enabled = vsync;

            // Create a DirectX graphic interface factory
            result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
            if (FAILED(result))
            {
                return false;
            }

            //Use the factory to create an adapter for the primary graphics interface
            result = factory->EnumAdapters(0, &adapter);
            if (FAILED(result))
            {
                return false;
            }

            //Enumurate the primary adapter output
            result = adapter->EnumOutputs(0, &adapterOutput);
            if (FAILED(result))
            {
                return false;
            }

            //Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM 
            //display format for the adapter output
            result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                                       DXGI_ENUM_MODES_INTERLACED,
                                                       &numModes,
                                                       NULL);
            if (FAILED(result))
            {
                return false;
            }

            //Create a list to hold all the possible modes for this monitor / video card 
            //combination
            displayModeList = new DXGI_MODE_DESC[numModes];

            //Fill the display mode list 
            result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
                                                       DXGI_ENUM_MODES_INTERLACED,
                                                       &numModes,
                                                       displayModeList);
            if (FAILED(result))
            {
                return false;
            }
                
            for (int i = 0; i < (int)numModes; ++i)
            {
                if (displayModeList[i].Width == (unsigned)width)
                {
                    if (displayModeList[i].Height == (unsigned)height)
                    {
                        numerator   = displayModeList[i].RefreshRate.Numerator;
                        denominator = displayModeList[i].RefreshRate.Denominator;
                    }
                }
            }

            if (numerator == 0 && denominator == 0)
            {
                return false;
            }

            //Get the adapter description
            result = adapter->GetDesc(&adapterDesc);
            if (FAILED(result))
            {
                return false;
            }

            //Store the video card memory in mbs
            m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

            //Convert the name of the video card to a character array
            //This is not really necessary...
            error = wcstombs_s(&strLen, m_videoCardDescription, 128, adapterDesc.Description, 128);
            if (error != 0)
            {
                return false;
            }

            //Release memories that we just allocated in function.
            delete[] displayModeList;
            displayModeList = 0;

            adapterOutput->Release();
            adapterOutput = 0;

            adapter->Release();
            adapter = 0;

            factory->Release();
            factory = 0;

            if (!InitializeSwapChain(hwnd, fullScreen, width, height, numerator, denominator))
            {
                return false;
            }

            //Get the pointer to the back buffer
            result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
            if (FAILED(result))
            {
                return false;
            }
            
            //Create the render target view
            result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
            if (FAILED(result))
            {
                return false;
            }
            
            backBufferPtr->Release();
            backBufferPtr = 0;

            if (!InitializeDepthBuffer(width, height))
            {
                return false;
            }

            if (!InitializeDepthBuffer(width, height))
            {
                return false;
            }

            if (!InitializeDepthStencilBuffer())
            {
                return false;
            }

            if (!InitializeStencilView())
            {
                return false;
            }

            //Bind the render target view and depth stencil buffer to the output render pipeline.
            m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

            if (!InitializeRasterizerState())
            {
                return false;
            }

            InitializeViewport(width, height);

            if (!InitializeAlphaBlending())
            {
                return false;
            }

            if (!InitializeZBuffer())
            {
                return false;
            }

            return true;
        }

        void DXManager::BeginScene(float r, float g, float b, float a)
        {
			float color[4];

			//red
			color[0] = r;
			//green
			color[1] = g;
			//blue
			color[2] = b;
			//alpha
			color[3] = a;

            //clear the back buffer
			m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

			//clear the depth buffer
			m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        }
        void DXManager::EndScene(void)
        {
			if (m_vsync_enabled)
			{
				//Lock to screen refresh rate.
				m_swapChain->Present(1, 0);
			}
			else
			{
				//Present as fast as possible.
				m_swapChain->Present(0, 0);
			}
        }

        void DXManager::EnableAlphaBlending(bool enable)
        {
            float blendFactor[4];
            blendFactor[0] = 0.0f;
            blendFactor[1] = 0.0f;
            blendFactor[2] = 0.0f;
            blendFactor[3] = 0.0f;

            if (enable)
            {
                m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
            }
            else
            {
                m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
            }
        }

        void DXManager::EnableZBuffer(bool enable)
        {
            if (enable)
            {
                m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
            }
            else
            {
                m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
            }
        }

        ID3D11Device* DXManager::GetDevice(void)
        {
            return m_device;
        }

        ID3D11DeviceContext* DXManager::GetDeviceContext(void)
        {
            return m_deviceContext;
        }

        //Private Methods       
        bool DXManager::InitializeSwapChain(HWND hwnd, bool fullScreen, int width, int height, unsigned numerator, unsigned denominator)
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            D3D_FEATURE_LEVEL    featureLevel;
            HRESULT              result;

            //Initialize swap chain
            ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

            //Set to a single back buffer
            swapChainDesc.BufferCount = 1;

            //Set the width and height of the back buffer
            swapChainDesc.BufferDesc.Width  = width;
            swapChainDesc.BufferDesc.Height = height;

            //Set regular 32-bit back buffer
            swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

            //Set the refresh rate of the back buffer
            if (m_vsync_enabled)
            {
                swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
                swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
            }
            else
            {           
                swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;         
                swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
            }

            //Set the usage of the back buffer
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

            //Set the handle for the window
            swapChainDesc.OutputWindow = hwnd;

            //turn multi-smapling off
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;

            //Set to full screen or windowed mode
            swapChainDesc.Windowed = fullScreen;

            //Set the scan line ordering and scaling to unspecified
            swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

            //Discard the back buffer contents after presenting
            swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

            swapChainDesc.Flags = 0;

            //Set the feature level to DirectX 11
            featureLevel = D3D_FEATURE_LEVEL_11_0;

            //Create swap chain, device, 
            result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
                D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

            if (FAILED(result))
            {
                return false;
            }

            return true;
        }

        bool DXManager::InitializeDepthBuffer(int width, int height)
        {
            D3D11_TEXTURE2D_DESC depthBufferDesc;
            HRESULT              result;

            //Initialize depth buffer desc
            ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

            //Set up depth buffer desc
            depthBufferDesc.Width  = width;
            depthBufferDesc.Height = height;
            depthBufferDesc.MipLevels = 1;
            depthBufferDesc.ArraySize = 1;
            depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; 
            depthBufferDesc.SampleDesc.Count = 1;
            depthBufferDesc.SampleDesc.Quality = 0;
            depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            depthBufferDesc.CPUAccessFlags = 0;
            depthBufferDesc.MiscFlags = 0;

            //Create the texture for the depth buffer
            result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
            if (FAILED(result))
            {
                return false;
            }


            return true;
        }

        bool DXManager::InitializeDepthStencilBuffer(void)
        {
            D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
            HRESULT result;

            //Initialize depth stencil desc;
            ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

            //Set up description
            depthStencilDesc.DepthEnable = true;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

            depthStencilDesc.StencilEnable = true;
            depthStencilDesc.StencilReadMask = 0xFF;
            depthStencilDesc.StencilWriteMask = 0xFF;

            //Stencil operation if pixel if front-facing
            depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
            depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            //Stencil operations if pixel is back-facing
            depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
            depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            //Create depth stencil state
            result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
            if (FAILED(result))
            {
                return false;
            }

            //Set depth stencil state
            m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

            return true;
        }

        bool DXManager::InitializeStencilView(void)
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
            HRESULT result;

            //initialize description;
            ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

            //set up description
            depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0;

            //Create depth stencil view
            result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
            if (FAILED(result))
            {
                return false;
            }


            return true;
        }

        bool DXManager::InitializeRasterizerState(void)
        {
            D3D11_RASTERIZER_DESC rasterDesc;
            HRESULT result;

            //Setup description
            rasterDesc.AntialiasedLineEnable = false;
            rasterDesc.CullMode = D3D11_CULL_BACK;
            rasterDesc.DepthBias = 0;
            rasterDesc.DepthBiasClamp = 0.0f;
            rasterDesc.DepthClipEnable = true;
            rasterDesc.FillMode = D3D11_FILL_SOLID;
            rasterDesc.FrontCounterClockwise = false;
            rasterDesc.MultisampleEnable = false;
            rasterDesc.ScissorEnable = false;
            rasterDesc.SlopeScaledDepthBias = 0.0f;

            //Create the rasterizer state
            result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
            if (FAILED(result))
            {
                return false;
            }

            //Set the rasterizer
            m_deviceContext->RSSetState(m_rasterState);

            return true;
        }

        void DXManager::InitializeViewport(int width, int height)
        {
            D3D11_VIEWPORT viewport;

            //Setup
            viewport.Width = (float)width;
            viewport.Height = (float)height;
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            viewport.TopLeftX = 0.0f;
            viewport.TopLeftY = 0.0f;

            //Create viewport
            m_deviceContext->RSSetViewports(1, &viewport);
        }

        bool DXManager::InitializeAlphaBlending(void)
        {
            D3D11_BLEND_DESC blendStateDesc;
            HRESULT result;

			//clear the blend state description
			ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

            //Initialize / clear description
            blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
            blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
            blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

            //Create the blend state
            result = m_device->CreateBlendState(&blendStateDesc, &m_alphaEnableBlendingState);
            if (FAILED(result))
            {
                return false;
            }

            //Modify to create disabled alpha blend state
            blendStateDesc.RenderTarget[0].BlendEnable = false;
            result = m_device->CreateBlendState(&blendStateDesc, &m_alphaDisableBlendingState);
            if (FAILED(result))
            {
                return false;
            }


            return true;
        }

        bool DXManager::InitializeZBuffer(void)
        {
            D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
            HRESULT result;

            //Clear the description
            ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

            //Set up description
            depthDisabledStencilDesc.DepthEnable = false;
            depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

            depthDisabledStencilDesc.StencilEnable = true;
            depthDisabledStencilDesc.StencilReadMask = 0xFF;
            depthDisabledStencilDesc.StencilWriteMask = 0xFF;

            //Stencil operation if pixel if front-facing
            depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
            depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            //Stencil operations if pixel is back-facing
            depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
            depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

            //Create disabled state
            result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
            if (FAILED(result))
            {
                return false;
            }

            return true;
        }
    }
}