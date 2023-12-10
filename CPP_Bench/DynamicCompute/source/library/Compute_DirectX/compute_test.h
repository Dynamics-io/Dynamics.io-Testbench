#pragma once

#include "stdafx.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11_4.h>

namespace DynamicCompute {
	namespace Compute {
		namespace DX {

			class compute_test {
			public:
				void Run() {
					InitDX();
					RunShader();
					CleanUp();
				}

			private:
				void InitDX();

				void RunShader();

				void CleanUp();

				ID3D11Device5* m_device{ nullptr };
				//ID3D11Device4* m_device4{ nullptr };
				ID3D11DeviceContext4* m_deviceContext{ nullptr };
				ID3D11ComputeShader* m_computeShader{ nullptr };
				ID3D11Buffer* m_in_buffer{ nullptr };
				ID3D11ShaderResourceView* m_in_buffer_view{ nullptr };
				ID3D11Buffer* m_out_buffer{ nullptr };
				ID3D11UnorderedAccessView* m_out_buffer_view{ nullptr };
				ID3D11Fence* fence;
			};
		}
	}
}
