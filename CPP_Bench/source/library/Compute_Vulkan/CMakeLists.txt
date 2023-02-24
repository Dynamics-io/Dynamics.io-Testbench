set(module "Compute_Vulkan")

set(${module}_headers
	#VK_forwardDeclarations.h
	vulkan_test.h
)

set(${module}_sources
	vulkan_test.cpp
)

add_library(${module} SHARED ${${module}_headers} ${${module}_sources})
target_link_libraries(Compute_Vulkan Vulkan-1 glfw3)

