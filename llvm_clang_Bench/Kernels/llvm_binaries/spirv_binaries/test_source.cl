

__kernel void bar(__global int * ptr) {
	size_t id = get_global_id(0);

	
	ptr[id] = 7;
}