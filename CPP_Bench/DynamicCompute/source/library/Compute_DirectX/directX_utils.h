#pragma once

#include "stdafx.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11_4.h>

namespace DynamicCompute {
    namespace Compute {
        namespace DX {
            namespace Utilities {

                std::vector<char> readFile(const std::string& filename);

                std::vector<IDXGIAdapter*> GetAdapters();

                void ReleaseAdapters(std::vector<IDXGIAdapter*>& adapters);

                IDXGIAdapter* GetAdapter(UINT index);

                HRESULT CreateDevice(IDXGIAdapter* adapter, ID3D11Device** device, ID3D11DeviceContext** deviceContext);


                D3D11_BUFFER_DESC get_BUFFER_DESC(UINT bindFlags, UINT MiscFlags, D3D11_USAGE usage, bool CPU_Read_Access, bool CPU_Write_Access, UINT numElements, UINT stride);

                D3D11_SUBRESOURCE_DATA get_SUBRESOURCE_DATA(void* data);

                D3D11_SHADER_RESOURCE_VIEW_DESC get_SHADER_RESOURCE_VIEW_DESC(ID3D11Buffer* ref, UINT firstElement);

                D3D11_UNORDERED_ACCESS_VIEW_DESC get_UNORDERED_ACCESS_VIEW_DESC(ID3D11Buffer* ref, UINT firstElement);


                HRESULT CreateBuffer(
                    ID3D11Device* device, 
                    UINT bindFlags, 
                    UINT MiscFlags, 
                    D3D11_USAGE usage, 
                    bool CPU_Read_Access, 
                    bool CPU_Write_Access, 
                    UINT numElements, 
                    UINT stride,
                    void* init_data,
                    ID3D11Buffer** buffer);

                HRESULT CreateStagingBuffer(
                    ID3D11Device* device,
                    ID3D11Buffer* ref,
                    bool CPU_Read_Access, 
                    bool CPU_Write_Access,
                    ID3D11Buffer** buffer
                );

                HRESULT CreateShaderResourceView(ID3D11Device* device, ID3D11Buffer* ref, UINT firstElement, ID3D11ShaderResourceView** view);

                HRESULT CreateUnorderedAccessView(ID3D11Device* device, ID3D11Buffer* ref, UINT firstElement, ID3D11UnorderedAccessView** view);


                void DispatchShaderKernel(
                    ID3D11DeviceContext* context, 
                    ID3D11ComputeShader* shader,
                    std::vector<ID3D11ShaderResourceView*>& resource_views, 
                    std::vector<ID3D11UnorderedAccessView*>& access_views,
                    UINT workgroups_x, UINT workgroups_y, UINT workgroups_z);
            }
        }
    }
}