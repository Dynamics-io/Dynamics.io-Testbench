
/*class Something {
  public:
    Something(int a) : v(a) {}
    int v;
};*/

__attribute__((reqd_work_group_size(16, 1, 1)))
kernel void work(global int * ptr) {
	size_t id = get_global_id(0);

	ptr[id] = (int)id;
}

__attribute__((reqd_work_group_size(16, 1, 1)))
kernel void work2(global int * ptr) {
	size_t id = get_global_id(0);

	//Something g(33);
	
	ptr[id] = (int)id;// + g.v;
}
