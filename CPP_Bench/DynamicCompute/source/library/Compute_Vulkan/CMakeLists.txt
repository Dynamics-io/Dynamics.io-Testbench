set(module "Compute_Vulkan")

set(${module}_headers
	#VK_forwardDeclarations.h
	vulkan_test.h
	vulkan_compute_test.h
	vulkan_utils.h
	ComputeEngine.h
	ComputeController_VK.h
	ComputeProgram_VK.h
	ComputeBuffer_VK.h
)

set(${module}_sources
	vulkan_test.cpp
	vulkan_compute_test.cpp
	vulkan_utils.cpp
	ComputeEngine.cpp
	ComputeController_VK.cpp
	ComputeProgram_VK.cpp
	ComputeBuffer_VK.cpp
)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})
target_link_libraries(Compute_Vulkan Vulkan-1 glfw3)

