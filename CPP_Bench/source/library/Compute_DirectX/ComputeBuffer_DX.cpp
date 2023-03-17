#include "ComputeBuffer_DX.h"

#include "Compute_DirectX/ComputeEngine.h"

using namespace Dynamics_IO_Testbench::Compute::DX;

int ComputeBuffer_DX::SetData(void* data)
{
	return m_buffer->SetData(data);
}

int ComputeBuffer_DX::GetData(void* outData)
{
	return m_buffer->GetData(outData);
}

size_t ComputeBuffer_DX::GetSize()
{
	return (size_t)m_buffer->GetFullSize();
}

void ComputeBuffer_DX::Dispose()
{
	m_buffer->Dispose();
}

void* ComputeBuffer_DX::Get_Native_Ptr()
{
	return m_buffer->Get_DX_Mem();
}
