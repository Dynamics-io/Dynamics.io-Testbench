
layout(local_size_x = 16)
kernel void work(global int * ptr) {
	size_t id = get_global_id(0);

	ptr[id] = (int)id;
}
