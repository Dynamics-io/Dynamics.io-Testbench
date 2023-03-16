project("lib_Dynamics_IO_Testbench")

set(${PROJECT_NAME}_headers
    dll_main.h
    ForwardDeclarations.h
    C_Variables.h
    Logger.h
    stdafx.h
)

set(${PROJECT_NAME}_sources
    dll_main.cpp
    C_Variables.cpp
    Logger.cpp
)

add_subdirectory(utils)
add_subdirectory(Compute_OCL)
add_subdirectory(Compute_CUDA)
add_subdirectory(Compute_Vulkan)
add_subdirectory(Compute_DirectX)
add_subdirectory(Compute_Interface)


add_library(lib_Dynamics_IO_Testbench SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})

target_link_libraries(lib_Dynamics_IO_Testbench Utils Compute_Interface Compute_CUDA Compute_OCL Compute_Vulkan Compute_DirectX)
