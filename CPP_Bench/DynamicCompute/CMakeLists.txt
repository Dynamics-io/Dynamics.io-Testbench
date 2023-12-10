cmake_minimum_required(VERSION 3.17)
project(DynamicCompute)

set(CMAKE_CXX_STANDARD 17)
set(CONAN_DISABLE_CHECK_COMPILER true)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (UNIX)
    add_compile_options(-pthread -fPIC)
endif ()

#find_package(OpenCL REQUIRED)
#message(${OpenCL_LIBRARY})
#message(${OpenCL_LIBRARY}/OpenCL.lib)
#message(${Vulkan_LIBRARY})
#message(${Vulkan_LIBRARY}/Vulkan-1.lib)
#link_directories(${OpenCL_LIBRARY})

message(${PROJECT_SOURCE_DIR})

set(Vulkan_LIBRARY ${PROJECT_SOURCE_DIR}/lib/vulkan_1.3.239.0/Lib)
link_directories(${Vulkan_LIBRARY})

set(OpenCL_SDK_Lib ${PROJECT_SOURCE_DIR}/lib/OpenCL_SDK/win32)
link_directories(${OpenCL_SDK_Lib})

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>)

add_subdirectory(source)



