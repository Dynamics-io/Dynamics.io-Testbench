
/*class Something {
  public:
    Something(int a) : v(a) {}
    int v;
};*/

global int* g_ptr;

__attribute__((reqd_work_group_size(16, 1, 1)))
kernel void work(global int * ptr) {
	size_t id = get_global_id(0);

	ptr[id] = (int)id + 6;
}

__attribute__((reqd_work_group_size(16, 1, 1)))
kernel void work2(global int * ptr) {
	size_t id = get_global_id(0);

	//Something g(33);
	
	ptr[id] = (int)id + 5;// + g.v;
}

void do_work3_test(size_t id, global int* ptr) {
	ptr[id] = id + 7;
}

void do_work4_test(size_t id) {
	g_ptr[id] = id + 7;
}

__attribute__((reqd_work_group_size(16, 1, 1)))
__kernel void work3(__global int * ptr) {
	size_t id = get_global_id(0);

	do_work3_test(id, ptr);
}

__attribute__((reqd_work_group_size(1, 1, 1)))
__kernel void work4(__global int * ptr) {
	//size_t id = get_global_id(0);

	g_ptr = ptr;

	for (int i = 0; i < 32; i++){
		do_work4_test(i);
	}
}

int LeadingZeros(int x)
{
	// from https://stackoverflow.com/questions/10439242/count-leading-zeroes-in-an-int32
	
	const int numIntBits = sizeof(int) * 8; //compile time constant
											//do the smearing
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	//count the ones
	x -= x >> 1 & 0x55555555;
	x = (x >> 2 & 0x33333333) + (x & 0x33333333);
	x = (x >> 4) + x & 0x0f0f0f0f;
	x += x >> 8;
	x += x >> 16;
	return numIntBits - (x & 0x0000003f); //subtract # of 1s from 32
	
	return 0;
}

int GetContainingPowerOf2(int i)
{
	int signed_ = i;//i == 0 ? 1 : i;
	if (i == 0)
		signed_ = 1;
	return (int)(32 - LeadingZeros(signed_ - 1));

}

__attribute__((reqd_work_group_size(16, 1, 1)))
__kernel void work5(__global int * ptr) {
	size_t id = get_global_id(0);

	ptr[id] = GetContainingPowerOf2(id);
}


__local int* p_ptr_g;
__local int* p_ptr2_g;

void DoWork6Loop()
{
	for (int i = 0; i < 32; i++){
		p_ptr_g[i] = GetContainingPowerOf2(i);
		p_ptr2_g[i] = p_ptr2_g[i] + 5;
	}
}

__attribute__((reqd_work_group_size(1, 1, 1)))
__kernel void work6(__global int * ptr, __global int * ptr2) {
	__local int p_ptr[32];
	__local int p_ptr2[32];
	
	p_ptr_g = p_ptr;
	p_ptr2_g = p_ptr2;
	
	//__local int p_ptr[32];
	
	//size_t id = get_global_id(0);

	event_t ev_in[2];
	ev_in[0] = async_work_group_copy (p_ptr, ptr, 32, 0);
	ev_in[1] = async_work_group_copy (p_ptr2, ptr2, 32, 0);
	wait_group_events(2, &ev_in);

	/*for (int i = 0; i < 32; i++){
		p_ptr[i] = GetContainingPowerOf2(i);
		p_ptr2[i] = ptr2[i] + 5;
	}*/
	DoWork6Loop();
	
	event_t ev_out[2];
	ev_out[0] = async_work_group_copy (ptr, p_ptr_g, 32, 0);
	ev_out[1] = async_work_group_copy (ptr2, p_ptr2_g, 32, 0);
	wait_group_events(2, &ev_out);
}