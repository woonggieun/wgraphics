#include "Shader.h"

namespace WE
{
	namespace Graphics
	{
		Shader::Shader(ID3D11Device* device, HWND hwnd,
			LPCSTR shaderFileName,
			LPCSTR vertexFuncName,
			LPCSTR pixelFuncName)
		{
			m_vertexShader = 0;
			m_pixelShader = 0;
			m_layout = 0;
			m_matrixBuffer = 0;

			m_isInitialized = Initialize(device, hwnd,
				shaderFileName,
				vertexFuncName,
				pixelFuncName);
		}

		Shader::~Shader(void)
		{
			// Release matrix buffer
			if (m_matrixBuffer)
			{
				m_matrixBuffer->Release();
				m_matrixBuffer = 0;
			}

			// Release layout
			if (m_layout)
			{
				m_layout->Release();
				m_layout = 0;
			}

			// Release pixel shader
			if (m_pixelShader)
			{
				m_pixelShader->Release();
				m_pixelShader = 0;
			}

			// Release vertex shader
			if (m_vertexShader)
			{
				m_vertexShader->Release();
				m_pixelShader = 0;
			}

			if (m_name)
			{
				delete[] m_name;
				m_name = nullptr;
			}
		}

		bool Shader::Initialize(ID3D11Device* device, HWND hwnd,
			                    LPCSTR shaderFileName,
			                    LPCSTR vertexFuncName,
			                    LPCSTR pixelFuncName)
		{
			bool result;

			size_t fileNameLen = strlen(shaderFileName);
			m_name = new char[fileNameLen + 1];
			
			memcpy(m_name, shaderFileName, fileNameLen + 1);

			char vsFileName[100];
			strcpy_s(vsFileName, shaderFileName);
			strcat_s(vsFileName, ".vers");
			vsFileName[fileNameLen + 6] = '\0';

			char psFileName[100];
			strcpy_s(psFileName, shaderFileName);
			strcat_s(psFileName, ".pixs");
			psFileName[fileNameLen + 6] = '\0';

			// Initialize the vertex and pixel shaders
			result = InitializeShader(device, hwnd, 
				                      vsFileName, psFileName, 
				                      vertexFuncName, pixelFuncName);

			return result;
		}

		bool Shader::InitializeShader(ID3D11Device* device, HWND hwnd,
			                          LPCSTR vsFileName,     LPCSTR psFileName,
									  LPCSTR vertexFuncName, LPCSTR pixelFuncName)
		{
			HRESULT result;
			ID3D10Blob* errorMessage = 0;
			ID3D10Blob* vertShaderBuffer;
			ID3D10Blob* pixShaderBuffer;
			D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
			D3D11_BUFFER_DESC matrixBufferDesc;
			unsigned int numElements;

			
		}

		void Shader::Begin(ID3D11DeviceContext* dc, int indexCount)
		{
			// Set the vertex input layout
			dc->IASetInputLayout(m_layout);
			
			// Set the vertex and pixel shaders that will be used to render
			dc->VSSetShader(m_vertexShader, NULL, 0);
			dc->PSSetShader(m_pixelShader, NULL, 0);

			// Render
			dc->DrawIndexed(indexCount, 0, 0); 
		}

		void Shader::End(ID3D11DeviceContext* dc)
		{
			dc->IASetInputLayout(NULL);			
			dc->VSSetShader(NULL, NULL, 0);
			dc->PSSetShader(NULL, NULL, 0);
		}

		bool Shader::SetShaderParameters(ID3D11DeviceContext* dc, 
			                             D3DXMATRIX worldMatrix,
										 D3DXMATRIX viewMatrix,
										 D3DXMATRIX projMatrx)
		{

		}

		bool Shader::SetShaderParameters(ID3D11DeviceContext* dc,
			                             ID3D11ShaderResourceView* texture)
		{

		}

		char* Shader::GetName(void)
		{
			return m_name;
		}

		bool Shader::IsInitialized(void)
		{
			return m_isInitialized;
		}


	}
}