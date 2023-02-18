set(module "Compute")

set(${module}_headers
	ComputeEngine.h
	ProgramBuilder.h
	ComputeController.h
	ShaderDepository.h
)

set(${module}_sources
	ComputeEngine.cpp
	ProgramBuilder.cpp
	ComputeController.cpp
	ShaderDepository.cpp
)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})

