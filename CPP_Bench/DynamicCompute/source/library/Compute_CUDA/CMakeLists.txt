set(module "Compute_CUDA")

set(${module}_headers

)

set(${module}_sources
	dummy.cpp
)

add_library(${module} STATIC ${${module}_headers} ${${module}_sources})

