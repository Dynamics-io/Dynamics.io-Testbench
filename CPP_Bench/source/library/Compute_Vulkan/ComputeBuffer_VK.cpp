#include "ComputeBuffer_VK.h"

#include "Compute_Vulkan/ComputeEngine.h"

using namespace Dynamics_IO_Testbench::Compute::VK;

int ComputeBuffer_VK::SetData(void* data)
{
	return m_buffer->SetData(data);
}

int ComputeBuffer_VK::GetData(void* outData)
{
	return m_buffer->GetData(outData);
}

size_t ComputeBuffer_VK::GetSize()
{
	return (size_t)m_buffer->GetSize();
}

void ComputeBuffer_VK::Dispose()
{
	m_buffer->Dispose();
}

void* ComputeBuffer_VK::Get_Native_Ptr()
{
	return m_buffer->Get_VK_Mem();
}
