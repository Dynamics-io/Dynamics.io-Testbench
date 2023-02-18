project("Dynamics_IO_Testbench")

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
add_subdirectory(Compute)


add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources})
target_link_libraries(${PROJECT_NAME} Utils Compute)
target_link_libraries (Dynamics_IO_Testbench OpenCL)