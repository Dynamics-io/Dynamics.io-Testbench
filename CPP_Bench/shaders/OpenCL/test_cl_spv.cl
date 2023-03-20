

__kernel void work(__global int * ptr) {
	size_t id = get_global_id(0);

	
	ptr[id] = 7;
}