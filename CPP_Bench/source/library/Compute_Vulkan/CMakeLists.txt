set(module "Compute_Vulkan")

set(${module}_headers
	#VK_forwardDeclarations.h
	vulkan_test.h
	vulkan_compute_test.h
	vulkan_utils.h
	ComputeEngine.h
)

set(${module}_sources
	vulkan_test.cpp
	vulkan_compute_test.cpp
	vulkan_utils.cpp
	ComputeEngine.cpp
)

add_library(${module} SHARED ${${module}_headers} ${${module}_sources})
target_link_libraries(Compute_Vulkan Vulkan-1 glfw3)

