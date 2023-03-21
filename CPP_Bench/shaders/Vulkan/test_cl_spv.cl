
__attribute__((reqd_work_group_size(16, 1, 1)))
kernel void work(global int * ptr) {
	size_t id = get_global_id(0);

	ptr[id] = (int)id;
}
