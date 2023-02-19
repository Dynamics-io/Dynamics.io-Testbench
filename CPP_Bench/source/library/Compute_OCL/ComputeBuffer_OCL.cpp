#include "ComputeBuffer_OCL.h"

using namespace Dynamics_IO_Testbench::Compute::OCL;

int Dynamics_IO_Testbench::Compute::OCL::ComputeBuffer_OCL::SetData(void* data)
{
	return m_buffer->SetData(data);
}

int Dynamics_IO_Testbench::Compute::OCL::ComputeBuffer_OCL::GetData(void* outData)
{
	return m_buffer->GetData(outData);
}

size_t Dynamics_IO_Testbench::Compute::OCL::ComputeBuffer_OCL::GetSize()
{
	return m_buffer->GetSize();
}

void Dynamics_IO_Testbench::Compute::OCL::ComputeBuffer_OCL::Dispose()
{
	m_buffer->Dispose();
}

void* Dynamics_IO_Testbench::Compute::OCL::ComputeBuffer_OCL::Get_Native_Ptr()
{
	return m_buffer->Get_CL_Mem();
}
