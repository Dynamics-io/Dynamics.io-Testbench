#include "directX_utils.h"

using namespace DynamicCompute::Compute::DX;

std::vector<char> Utilities::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

std::vector<IDXGIAdapter*> Utilities::GetAdapters()
{
    IDXGIFactory* factory;
    std::vector<IDXGIAdapter*> adapters;
    DXGI_ADAPTER_DESC2 desc;
    IDXGIAdapter* tmpAdapter;

    HRESULT res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(res)) {
        return adapters;
    }

    for (UINT i = 0; factory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmpAdapter);
    }

    // Release the factory.
    factory->Release();
    factory = nullptr;

    return adapters;
}

void Utilities::ReleaseAdapters(std::vector<IDXGIAdapter*>& adapters)
{
    for (auto& adpt : adapters) {
        adpt->Release();
    }

    adapters.clear();
}

IDXGIAdapter* Utilities::GetAdapter(UINT index)
{
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;

    HRESULT res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(res)) {
        return nullptr;
    }

    res = factory->EnumAdapters(index, &adapter);
    if (FAILED(res)) {
        return nullptr;
    }

    // Release the factory.
    factory->Release();
    factory = nullptr;

    return adapter;
}

HRESULT Utilities::CreateDevice(IDXGIAdapter* adapter, ID3D11Device** device, ID3D11DeviceContext** deviceContext)
{
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    HRESULT res = D3D11CreateDevice(
        adapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        NULL,
        0,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        device,
        NULL,
        deviceContext
    );

    return res;
}

D3D11_BUFFER_DESC Utilities::get_BUFFER_DESC(UINT bindFlags, UINT MiscFlags, D3D11_USAGE usage, bool CPU_Read_Access, bool CPU_Write_Access, UINT numElements, UINT stride)
{
    UINT cpu_flags = 0;
    if (CPU_Read_Access)
        cpu_flags |= D3D11_CPU_ACCESS_READ;
    if (CPU_Write_Access)
        cpu_flags |= D3D11_CPU_ACCESS_WRITE;

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.BindFlags = bindFlags;
    buffer_desc.MiscFlags = MiscFlags;
    buffer_desc.Usage = usage;
    buffer_desc.CPUAccessFlags = cpu_flags;
    buffer_desc.ByteWidth = numElements * stride;
    buffer_desc.StructureByteStride = stride;

    return buffer_desc;
}

D3D11_SUBRESOURCE_DATA Utilities::get_SUBRESOURCE_DATA(void* data)
{
    D3D11_SUBRESOURCE_DATA init_data;
    init_data.pSysMem = data;

    return init_data;
}

D3D11_SHADER_RESOURCE_VIEW_DESC Utilities::get_SHADER_RESOURCE_VIEW_DESC(ID3D11Buffer* ref, UINT firstElement)
{
    D3D11_BUFFER_DESC descBuf{};
    ref->GetDesc(&descBuf);

    D3D11_SHADER_RESOURCE_VIEW_DESC descView{};
    descView.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
    descView.BufferEx.FirstElement = firstElement;

    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

    return descView;
}

D3D11_UNORDERED_ACCESS_VIEW_DESC Utilities::get_UNORDERED_ACCESS_VIEW_DESC(ID3D11Buffer* ref, UINT firstElement)
{
    D3D11_BUFFER_DESC descBuf{};
    ref->GetDesc(&descBuf);

    D3D11_UNORDERED_ACCESS_VIEW_DESC descView{};
    descView.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    descView.Buffer.FirstElement = firstElement;

    descView.Format = DXGI_FORMAT_UNKNOWN;
    descView.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;

    return descView;
}

HRESULT Utilities::CreateBuffer(
    ID3D11Device* device, 
    UINT bindFlags, 
    UINT MiscFlags, 
    D3D11_USAGE usage, 
    bool CPU_Read_Access, bool CPU_Write_Access, 
    UINT numElements, UINT stride, 
    void* init_data, 
    ID3D11Buffer** buffer)
{
    D3D11_BUFFER_DESC buffer_desc = Utilities::get_BUFFER_DESC(
        bindFlags,
        MiscFlags,
        usage,
        CPU_Read_Access,
        CPU_Write_Access,
        numElements,
        stride
    );

    // subresource data object needs to be NULL if init_data is NULL, or it will not create the buffer correctly.
    D3D11_SUBRESOURCE_DATA* init_data_ptr = nullptr;
    D3D11_SUBRESOURCE_DATA init_data_desc;
    if (init_data != nullptr) {
        init_data_desc.pSysMem = init_data;
        init_data_ptr = &init_data_desc;
    }

    HRESULT res = device->CreateBuffer(&buffer_desc, init_data_ptr, buffer);


    return res;
}

HRESULT DynamicCompute::Compute::DX::Utilities::CreateStagingBuffer(
    ID3D11Device* device, 
    ID3D11Buffer* ref, 
    bool CPU_Read_Access, 
    bool CPU_Write_Access,
    ID3D11Buffer** buffer)
{
    UINT cpu_flags = 0;
    if (CPU_Read_Access)
        cpu_flags |= D3D11_CPU_ACCESS_READ;
    if (CPU_Write_Access)
        cpu_flags |= D3D11_CPU_ACCESS_WRITE;

    D3D11_BUFFER_DESC desc{};
    ref->GetDesc(&desc);

    desc.CPUAccessFlags = cpu_flags;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.MiscFlags = 0;

    HRESULT res = device->CreateBuffer(&desc, NULL, buffer);

    return res;
}

HRESULT Utilities::CreateShaderResourceView(ID3D11Device* device, ID3D11Buffer* ref, UINT firstElement, ID3D11ShaderResourceView** view)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc = Utilities::get_SHADER_RESOURCE_VIEW_DESC(ref, firstElement);

    HRESULT res = device->CreateShaderResourceView(ref, &view_desc, view);

    return res;
}

HRESULT Utilities::CreateUnorderedAccessView(ID3D11Device* device, ID3D11Buffer* ref, UINT firstElement, ID3D11UnorderedAccessView** view)
{
    D3D11_UNORDERED_ACCESS_VIEW_DESC view_desc = Utilities::get_UNORDERED_ACCESS_VIEW_DESC(ref, firstElement);

    HRESULT res = device->CreateUnorderedAccessView(ref, &view_desc, view);

    return res;
}

void Utilities::DispatchShaderKernel(
    ID3D11DeviceContext* context, 
    ID3D11ComputeShader* shader,
    std::vector<ID3D11ShaderResourceView*>& resource_views, 
    std::vector<ID3D11UnorderedAccessView*>& access_views, 
    UINT workgroups_x, UINT workgroups_y, UINT workgroups_z)
{
    // Some service variables
    //ID3D11UnorderedAccessView* ppUAViewNULL[1] = {NULL};
    //ID3D11ShaderResourceView* ppSRVNULL[2] = { NULL, NULL };
    std::vector<ID3D11UnorderedAccessView*> ppUAViewNULL(resource_views.size());
    std::vector<ID3D11ShaderResourceView*> ppSRVNULL(access_views.size());

    context->CSSetShader(shader, NULL, 0);
    context->CSSetShaderResources(0, resource_views.size(), resource_views.data());
    context->CSSetUnorderedAccessViews(0, access_views.size(), access_views.data(), NULL);

    context->Dispatch(workgroups_x, workgroups_y, workgroups_z);

    context->CSSetShader(NULL, NULL, 0);
    context->CSSetUnorderedAccessViews(0, ppUAViewNULL.size(), ppUAViewNULL.data(), NULL);
    context->CSSetShaderResources(0, ppSRVNULL.size(), ppSRVNULL.data());
}
