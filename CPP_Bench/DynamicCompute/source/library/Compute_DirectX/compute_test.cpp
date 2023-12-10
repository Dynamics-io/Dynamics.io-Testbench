#include "compute_test.h"

#include <cstdlib>

#include "directX_utils.h"

using namespace DynamicCompute::Compute::DX;
using namespace DirectX;

#define INPUT_SIZE 16
#define WORK_GROUP_SIZE 16

void compute_test::InitDX()
{
	HRESULT res;

	IDXGIFactory* factory;
	
	IDXGIOutput* adapterOutput;

	// Create a DirectX graphics interface factory.
	res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(res)) {
		return;
	}

	DXGI_ADAPTER_DESC2 desc;
	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter;
	// Use the factory to create an adapter for the primary graphics interface (video card).
	for (UINT i = 0; factory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
		ZeroMemory(&desc, sizeof(desc));
		tmpAdapter->GetDesc((DXGI_ADAPTER_DESC*)&desc);

		//std::string desc_name;
		//strcpy(desc_name, (char*)desc.Description);
		//std::strcpy(desc_name, (char*)desc.Description);

		//std::wcout << "Device: " << desc.Description << std::endl;
		std::wstring wdesc_name(desc.Description);
		std::string desc_name(wdesc_name.begin(), wdesc_name.end());
		printf("Device name: %s\n", desc_name.c_str());
	}

	

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	res = Utilities::CreateDevice(adapters[0], (ID3D11Device**)&m_device, (ID3D11DeviceContext**)&m_deviceContext);


	if (FAILED(res)) {
		printf("Failed to create DirectX device: %X\n", res);
		throw std::exception("Failed to create DirectX device.");
	}

	int error;
	ID3D10Blob* shaderBuffer;
	ID3D10Blob* errorMessage;

	wchar_t vsFilename[128];
	error = wcscpy_s(vsFilename, 128, L"C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/DirectX/compute.hlsl");

	LPCSTR pProfile = (m_device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	

	/*res = D3DCompileFromFile(
		vsFilename,
		NULL,
		NULL,
		"CSMain",
		pProfile,
		dwShaderFlags,
		0,
		&shaderBuffer,
		&errorMessage);

	if (FAILED(res)) {
		printf("Compile ERROR: %s", (char*)errorMessage->GetBufferPointer());
		throw std::exception("Failed to compile compute file.");
	}*/

	std::string shader_directory = "C:/Users/jdrurka1/source/repos/Dynamics-io/Dynamics.io-Testbench/CPP_Bench/shaders/DirectX/";
	auto computeShaderBin = Utilities::readFile(shader_directory + "compute.fxc");

	res = m_device->CreateComputeShader(computeShaderBin.data(), computeShaderBin.size(), NULL, &m_computeShader);
	//m_device->CreateComputeShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), NULL, &m_computeShader);
	if (FAILED(res)) {
		printf("Error making shader: %il\n", res);
		throw std::exception("Failed to create compute shader!");
	}

	/////////////
	// In Buffer

	float* inBuff = new float[INPUT_SIZE];
	for (int i = 0; i < INPUT_SIZE; i++) {
		inBuff[i] = i + 1;
	}

	res = Utilities::CreateBuffer(
		m_device,
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		D3D11_USAGE_DEFAULT,
		false,
		false,
		INPUT_SIZE,
		sizeof(float),
		inBuff,
		&m_in_buffer);
	if (FAILED(res) || m_in_buffer == nullptr) {
		printf("Failed to create in buffer: %X\n", res);
		throw std::exception("Failed to create in buffer.");
	}


	res = Utilities::CreateShaderResourceView(m_device, m_in_buffer, 0, &m_in_buffer_view);
	if (FAILED(res)) {
		throw std::exception("Failed to create in buffer view.");
	}

	D3D11_BUFFER_DESC descBuf{};


	/////////////
	// Out Buffer

	res = Utilities::CreateBuffer(
		m_device,
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		D3D11_USAGE_DEFAULT,
		false,
		false,
		INPUT_SIZE,
		sizeof(float),
		nullptr,
		&m_out_buffer
	);
	if (FAILED(res) || m_in_buffer == nullptr) {
		throw std::exception("Failed to create out buffer.");
	}


	res = Utilities::CreateUnorderedAccessView(m_device, m_out_buffer, 0, &m_out_buffer_view);
	if (FAILED(res)) {
		throw std::exception("Failed to create out buffer view.");
	}

	m_device->CreateFence(0, D3D11_FENCE_FLAG_NONE, __uuidof(ID3D11Fence), (void**)&fence);

	printf("Initialization of Device and buffers finished.\n");
}

void compute_test::RunShader()
{

	std::vector<ID3D11ShaderResourceView*> resource_views(1);
	resource_views[0] = (m_in_buffer_view);

	std::vector<ID3D11UnorderedAccessView*> access_views(1);
	access_views[0] = (m_out_buffer_view);
	
	Utilities::DispatchShaderKernel(
		m_deviceContext, m_computeShader,
		resource_views,
		access_views,
		INPUT_SIZE / WORK_GROUP_SIZE, 1, 1);




	printf("execute kernel\n");
	//Sleep(1000);

	ID3D11Buffer* debugbuf = NULL;


	HRESULT res = Utilities::CreateStagingBuffer(m_device, m_out_buffer, true, false, &debugbuf);
	if (FAILED(res)) {
		printf("Failed to create staging buffer: %X\n", res);
		throw std::exception("Failed to create staging buffer.");
	}

	m_deviceContext->CopyResource(debugbuf, m_out_buffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	res = m_deviceContext->Map(debugbuf, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(res)) {
		printf("Failed to map out buffer: %X\n", res);
		throw std::exception("Failed to map outbuffer");
	}

	float* outBuff = new float[INPUT_SIZE];
	memcpy(outBuff, mappedResource.pData, INPUT_SIZE * sizeof(float));

	m_deviceContext->Unmap(debugbuf, 0);

	for (int i = 0; i < INPUT_SIZE; i++) {
		printf("res %i: %f\n", i, outBuff[i]);
	}

	delete[] outBuff;
}

void compute_test::CleanUp()
{
}
