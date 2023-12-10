project("DynamicComputeLibrary")

set(${module}_headers
	std_includes.h
	dynamic_compute.h
	InterfaceIncludes.h
	PlatformStructures.h
	IComputeBuffer.h
	IComputeProgram.h
	IComputeController.h
	ComputeInterface.h
)

set(${module}_sources
	PlatformStructures.cpp
	IComputeBuffer.cpp
	IComputeProgram.cpp
	IComputeController.cpp
	ComputeInterface.cpp	
)


#add_library(${PROJECT_NAME} SHARED ${${module}_headers} ${${module}_sources})
add_library(${PROJECT_NAME} STATIC ${${module}_headers} ${${module}_sources})
target_link_libraries(${PROJECT_NAME} libDynamicCompute)


#add_library(${module} STATIC ${${module}_headers} ${${module}_sources})

# Compute Interface does not directly utilize an OpenCL compute device,
# however the OpenCl library is used to processes platform and device information
# used to determine which specific Compute SDK to use. Currently, Only
# OpenCl (OCL) and CUDA is to be initially supported.

#target_link_libraries(DynamicComputeLibrary OpenCL)
#target_link_libraries(Compute_Interface OpenCL-SDK OpenCLUtils OpenCLUtilsCpp)
