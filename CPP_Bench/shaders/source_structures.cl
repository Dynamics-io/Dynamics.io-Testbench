#exclude

struct ground_model_t {
	float va;                       //!< adhesion velocity
	float ms;                       //!< static friction coefficient
	float mc;                       //!< sliding friction coefficient
	float t2;                       //!< hydrodynamic friction (s/m)
	float vs;                       //!< stribeck velocity (m/s)
	float alpha;                    //!< steady-steady
	float strength;                 //!< ground strength
	
	float solid_ground_level;       //!< how deep the solid ground is
    float drag_anisotropy;          //!< Upwards/Downwards drag anisotropy
};

struct node_t {
	float3 RelPosition;             //!< relative to the local physics origin (one origin per actor) (shaky)
	float3 AbsPosition;             //!< absolute position in the world (shaky)
	float3 Velocity;
	float3 Forces;
	
	float mass;
	float buoyancy;
	float friction_coef;
	float surface_coef;
	float volume_coef;
	
	int pos;
	// coll box id
	// lock group
	
	int bitFlags;
	
	// nd_cab_node;
	// nd_contacter;
	// nd_contactable;
	// nd_has_ground_contact;
	// nd_has_mesh_contact;
	// nd_immovable;
	// nd_loaded_mass;
	// nd_no_ground_contact;
	
	float nd_avg_collision_slip;
	float3 nd_last_collision_slip;
	float3 nd_last_collision_force;
	int nd_last_collision_gm;
};

struct beam_t
{
	int actor_id;
	int p1; // local node id
	int p2; // local node_id
	
	float k; //!< tensile spring
	float d; //!< damping factor
	float L; //!< length
	
	float minmaxposnegstress;
	float maxposstress;
	float maxnegstress;
	float strength;
	float stress;
	float plastic_coef;
	
	int detacher_group;
	int bounded; // SpecialBeam
	int bm_type;
	bool bm_inter_actor;
	int bm_locked_actor;
	
	bool bm_disabled;
	bool bm_broken;
	
	float shortbound;
	float longbound;
	float refL;
	
	// shock
};
