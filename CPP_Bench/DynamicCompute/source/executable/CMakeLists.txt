project("Dynamics_io_tester")

#set(${PROJECT_NAME}_headers
#)

set(${PROJECT_NAME}_sources
    exe_main.cpp
)

#add_executable(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_headers} ${${PROJECT_NAME}_sources}
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources})

target_link_libraries(${PROJECT_NAME} libDynamicCompute)
