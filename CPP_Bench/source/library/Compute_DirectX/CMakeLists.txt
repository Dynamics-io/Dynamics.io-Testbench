set(module "Compute_DirectX")

set(${module}_headers
	directX_utils.h
	SystemClass_test.h
	GraphicsClass_test.h
	InputClass_test.h
	d3dclass_test.h
	compute_test.h
	#ComputeEngine.h
	#ComputeController_DX.h
	#ComputeProgram_DX.h
	#ComputeBuffer_DX.h
)

set(${module}_sources
	directX_utils.cpp
	systemclass_test.cpp
	GraphicsClass_test.cpp
	InputClass_test.cpp
	d3dclass_test.cpp
	compute_test.cpp
	#ComputeEngine.cpp
	#ComputeController_DX.cpp
	#ComputeProgram_DX.cpp
	#ComputeBuffer_DX.cpp
)

add_library(${module} SHARED ${${module}_headers} ${${module}_sources})
target_link_libraries(Compute_DirectX glfw3)

