set(module "Compute_OCL")

set(${module}_headers
	OCL_forwardDeclarations.h
	ComputeEngine.h
	ShaderDepository.h
	ProgramBuilder.h
	#ComputeController.h
	ComputeController_OCL.h
	ComputeProgram_OCL.h
	ComputeBuffer_OCL.h
	
	#compute_test.h
)

set(${module}_sources
	ComputeEngine.cpp
	ShaderDepository.cpp
	ProgramBuilder.cpp
	#ComputeController.cpp
	ComputeController_OCL.cpp
	ComputeProgram_OCL.cpp
	ComputeBuffer_OCL.cpp
	
	#compute_test.cpp
)

add_subdirectory(CL_SDK)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})
target_link_libraries(Compute_OCL Compute_OCL_SDK)

