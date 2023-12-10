set(module "Compute_Interface")

set(${module}_headers
	InterfaceIncludes_private.h
	PlatformStructures_private.h
	ComputeInterface_private.h
	IComputeController_private.h
	IComputeProgram_private.h
	IComputeBuffer_private.h
)

set(${module}_sources
	ComputeInterface.cpp
)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})

# Compute Interface does not directly utilize an OpenCL compute device,
# however the OpenCl library is used to processes platform and device information
# used to determine which specific Compute SDK to use. Currently, Only
# OpenCl (OCL) and CUDA is to be initially supported.

#target_link_libraries(Compute_Interface OpenCL)
target_link_libraries(Compute_Interface OpenCL-SDK OpenCLUtils OpenCLUtilsCpp)
