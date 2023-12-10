#include "Compute_DirectX/ComputeEngine.h"

using namespace DynamicCompute::Compute::DX;

bool ComputeEngine::mInitialized = false;
std::string ComputeEngine::mApp_dir = "";
std::list<ComputeContext> ComputeEngine::mContexts;


int ComputeEngine::Init(std::string dir) {
	mApp_dir = dir;
	mInitialized = true;
	return 0;
}

ComputeContext* ComputeEngine::GetNewContext(DirectX_Device_Info device)
{
	mContexts.emplace_back(ComputeContext(device));
	auto& buf = mContexts.back();
	buf.mCanCallDispose = true;
	return &buf;
}

void ComputeEngine::Dispose() {
	mContexts.clear();
}


ComputeContext::ComputeContext(DirectX_Device_Info device) {
	HRESULT res;

	IDXGIFactory* factory;

	// Create a DirectX graphics interface factory.
	res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(res)) {
		return;
	}

	DXGI_ADAPTER_DESC2 adapter_desc;
	IDXGIAdapter* adapter;

	res = factory->EnumAdapters(device.AdapterIndex, &adapter);
	if (FAILED(res)) {
		return;
	}

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	res = Utilities::CreateDevice(adapter, (ID3D11Device**)&m_device, (ID3D11DeviceContext**)&m_deviceContext);
	if (FAILED(res)) {
		return;
	}

	factory->Release();
	adapter->Release();
}

ComputeProgram* ComputeContext::Add_Program(std::string name) {
	programs[name] = ComputeProgram(name, this);
	programs[name].mCanCallDispose = true;
	return &programs[name];
}

ComputeProgram* ComputeContext::Add_Program_Source(std::string name, const char* source) {
	if (programs.count(name) == 0)
	{
		Add_Program(name);
	}
	int res = programs[name].Set_Source(source);
	return &programs[name];
}

ComputeProgram* ComputeContext::Add_Program_Binary(std::string name, const void* binary, size_t length) {
	if (programs.count(name) == 0)
	{
		Add_Program(name);
	}
	int res = programs[name].Set_Binary(binary, length);
	return &programs[name];
}

ComputeProgram* ComputeContext::Add_Program_Binary_File(std::string name, std::string file_path) {
	if (programs.count(name) == 0)
	{
		Add_Program(name);
	}
	int res = programs[name].Set_Binary_File(file_path);
	return &programs[name];
}

ComputeProgram* ComputeContext::Programs(std::string name) {
	if (programs.count(name) > 0) {
		return &programs[name];
	}
	return nullptr;
}

ComputeKernel* ComputeContext::GetKernel(std::string p_name, std::string name) {
	return Programs(p_name)->GetKernel(name);
}

ComputeBuffer* ComputeContext::CreateBuffer(ComputeBuffer::Buffer_Type type, UINT numElements, UINT stride) {
	mBuffers.emplace_back(ComputeBuffer(this, type, numElements, stride));
	auto& buf = mBuffers.back();
	buf.mCanCallDispose = true;
	return &buf;
}

void ComputeContext::Dispose() {
	if (mDestroyed || !mCanCallDispose)
		return;

	programs.clear();
	mBuffers.clear();

	m_device->Release();
	m_device = nullptr;

	m_deviceContext->Release();
	m_deviceContext = nullptr;

	mDestroyed = true;
}

ComputeContext::~ComputeContext() {
	Dispose();
}



ComputeProgram::ComputeProgram(std::string name, ComputeContext* context) {
	mName = name;
	mContext = context;
	mDirectory = ComputeEngine::GetAppDir() + name + "\\";
	mInitialized = true;
}

// Not used as source is obtained at the kernel level.
int ComputeProgram::Set_Source(const char* source) {
	return 0;
}

// Not used as source is obtained at the kernel level.
int ComputeProgram::Set_Binary(const void* binary, size_t length) {
	return 0;
}

// Not used as source is obtained at the kernel level.
int ComputeProgram::Set_Binary_File(std::string file_path) {
	return 0;
}

ComputeKernel* ComputeProgram::GetKernel(std::string name) {
	if (kernels.count(name) > 0)
	{
		return &kernels[name];
	}

	kernels[name] = ComputeKernel(name, this);
	kernels[name].mCanCallDispose = true;
	return &kernels[name];
}

int ComputeProgram::Buildkernels() {
	HRESULT res = 0;

	for (auto& [key, value] : kernels) {
		res = value.BuildKernel();

		if (FAILED(res))
			break;
	}

	return (int)res;
}

void ComputeProgram::Dispose() {
	if (mDestroyed || !mInitialized || !mCanCallDispose)
		return;

	kernels.clear();

	mDestroyed = true;
	mInitialized = false;
}

ComputeProgram::~ComputeProgram() {
	Dispose();
}


ComputeKernel::ComputeKernel(std::string name, ComputeProgram* program) {
	mName = name;
	mProgram = program;
	m_device = program->GetContext()->GetDevice();
	m_deviceContext = program->GetContext()->GetDeviceContext();

	mFilePath = program->GetDirectory() + name + "." + mFileExtension;
}

int ComputeKernel::SetBuffer(ComputeBuffer* buffer, int arg) {
	int res;
	
	switch (buffer->GetType()) {
	case ComputeBuffer::Buffer_Type::READ:
		res = addResourceView(buffer, arg);
		break;

	case ComputeBuffer::Buffer_Type::Read_Write:
	case ComputeBuffer::Buffer_Type::Write:
		res = addUnorderedAccessView(buffer, arg);
		break;
	}

	return res;
}

HRESULT ComputeKernel::BuildKernel() {
	auto computeShaderBin = Utilities::readFile(mFilePath);

	HRESULT res = m_device->CreateComputeShader(computeShaderBin.data(), computeShaderBin.size(), NULL, &m_computeShader);
	if (!FAILED(res)) {
		mInitialized = true;
	}

	return res;
}

int ComputeKernel::Execute(UINT x, UINT y, UINT z) {
	if (!mInitialized)
		return -1;

	Utilities::DispatchShaderKernel(
		m_deviceContext, m_computeShader,
		mResourceViews,
		mAccessViews,
		std::max((int)x / (int)mWorkGroupSize, 1),
		std::max((int)y / (int)mWorkGroupSize, 1),
		std::max((int)z / (int)mWorkGroupSize, 1)
	);

	return 0;
}

void ComputeKernel::Dispose() {
	if (mDestroyed || !mInitialized || !mCanCallDispose)
		return;

	for (auto& entry : mResourceViews) {
		entry->Release();
	}

	for (auto& entry : mAccessViews) {
		entry->Release();
	}

	mResourceViews.clear();
	mAccessViews.clear();

	m_computeShader->Release();
	m_computeShader = nullptr;

	mDestroyed = true;
	mInitialized = false;
}

HRESULT ComputeKernel::addResourceView(ComputeBuffer* buffer, int arg)
{
	if (arg >= mResourceViews.size()) {
		mResourceViews.resize(arg + 1);
	}

	ID3D11ShaderResourceView* buffer_view;
	HRESULT res = Utilities::CreateShaderResourceView(m_device, buffer->Get_DX_Mem(), 0, &buffer_view);
	if (FAILED(res)) {
		return res;
	}

	mResourceViews[arg] = buffer_view;

	return res;
}

HRESULT ComputeKernel::addUnorderedAccessView(ComputeBuffer* buffer, int arg)
{
	if (arg >= mAccessViews.size()) {
		mAccessViews.resize(arg + 1);
	}

	ID3D11UnorderedAccessView* buffer_view;
	HRESULT res = Utilities::CreateUnorderedAccessView(m_device, buffer->Get_DX_Mem(), 0, &buffer_view);
	if (FAILED(res)) {
		return res;
	}

	mAccessViews[arg] = buffer_view;

	return res;
}

ComputeKernel::~ComputeKernel() {
	Dispose();
}


ComputeBuffer::ComputeBuffer(ComputeContext* context, Buffer_Type type, UINT numElements, UINT stride) {
	mContext = context;
	mType = type;
	mNumElements = numElements;
	mStride = stride;

	m_device = context->GetDevice();
	m_deviceContext = context->GetDeviceContext();


	HRESULT res = Utilities::CreateBuffer(
		m_device,
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		D3D11_USAGE_DEFAULT,
		false,
		false,
		mNumElements,
		mStride,
		nullptr,
		&mBuffer
	);
	if (FAILED(res) || mBuffer == nullptr) {
		return;
	}

	bool CPU_read = false;
	bool CPU_write = false;

	switch (mType) {
	case Buffer_Type::READ:
		// Compute shader reads from buffer, thus CPU writes to it.
		CPU_write = true;
		break;

	case Buffer_Type::Write:
		// Compute shader writes to buffer, thus CPU reads from it.
		CPU_read = true;
		break;

	case Buffer_Type::Read_Write:
		// Compute shader reads or writes to it, thus CPU should be either to do either.
		CPU_read = true; 
		CPU_write = true;
		break;
	}

	res = Utilities::CreateStagingBuffer(
		m_device, 
		mBuffer, 
		CPU_read,
		CPU_write,
		&mStagingBuffer
	);
	if (FAILED(res)) {
		return;
	}

}

int ComputeBuffer::SetData(void* data) {
	// data is expected to be pre-allocated.

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//map
	HRESULT res = m_deviceContext->Map(mStagingBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource);
	if (FAILED(res)) {
		return res;
	}

	// copy from region
	memcpy(mappedResource.pData, data, mNumElements * mStride);

	// unmap
	m_deviceContext->Unmap(mStagingBuffer, 0);
	
	m_deviceContext->CopyResource(mBuffer, mStagingBuffer);

	return res;
}

int ComputeBuffer::GetData(void* outData) {
	// outData is expected to be pre-allocated.

	m_deviceContext->CopyResource(mStagingBuffer, mBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// map
	HRESULT res = m_deviceContext->Map(mStagingBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(res)) {
		return res;
	}

	// copy to region
	memcpy(outData, mappedResource.pData, mNumElements * mStride);

	// unmap
	m_deviceContext->Unmap(mStagingBuffer, 0);

	return res;
}

void ComputeBuffer::Dispose() {
	if (mDestroyed || !mCanCallDispose)
		return;

	mStagingBuffer->Release();
	mStagingBuffer = nullptr;

	mBuffer->Release();
	mBuffer = nullptr;

	mDestroyed = true;
}

ComputeBuffer::~ComputeBuffer() {
	Dispose();
}
