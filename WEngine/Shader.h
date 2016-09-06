#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>

namespace WE
{
	namespace Graphics
	{
		class Shader
		{
		private:
			struct MatrixBufferType
			{
				D3DXMATRIX worldMatrix;
				D3DXMATRIX viewMatrix;
				D3DXMATRIX projMatrix;
			};

		public:
			Shader(ID3D11Device* device, HWND hwnd,
				LPCSTR shaderFileName,
				LPCSTR vertexFuncName,
				LPCSTR pixelFuncName);

			virtual ~Shader(void);

			virtual void Begin(ID3D11DeviceContext* dc, int indexCount);
			virtual void End(ID3D11DeviceContext* dc);

			bool SetShaderParameters(ID3D11DeviceContext* dc,
				D3DXMATRIX           worldMatrix,
				D3DXMATRIX           viewMatrix,
				D3DXMATRIX           projMatrix);

			bool SetShaderParameters(ID3D11DeviceContext* dc,
				ID3D11ShaderResourceView* texture);

			char* GetName(void);
			bool  IsInitialized(void);


		protected:
			virtual bool Initialize(ID3D11Device* device, HWND hwnd,
				LPCSTR shaderFileName,
				LPCSTR vertexFuncName,
				LPCSTR pixelFuncName);


		private:
			bool InitializeShader(ID3D11Device* device, HWND hwnd,
				LPCSTR vsFileName,     LPCSTR psFileName,
				LPCSTR vertexFuncName, LPCSTR pixelFuncName);

			void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCSTR shaderFileName);

			ID3D11VertexShader* m_vertexShader;
			ID3D11PixelShader*  m_pixelShader;
			ID3D11InputLayout*  m_layout;
			ID3D11Buffer*       m_matrixBuffer;
			char*               m_name;
			bool                m_isInitialized;

		};
	}
}
#endif