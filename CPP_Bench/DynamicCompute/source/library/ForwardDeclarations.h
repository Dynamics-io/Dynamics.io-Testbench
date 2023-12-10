#pragma once

//#include "stdafx.h"

struct C_Vec2;
struct C_Vec3;
struct C_Vec4;
struct C_Quat;

namespace DynamicCompute {


	namespace Compute {
		
		struct Platform;
		struct Device;

		class IComputeBuffer_private;
		class IComputeProgram_private;
		class IComputeController_private;
		class ComputeInterface_private;

		class IComputeBuffer;
		class IComputeProgram;
		class IComputeController;
		class ComputeInterface;

		namespace OCL {

			class ComputeContext;
			class ComputeEngine;
			class ComputeKernel;
			class ComputeBuffer;

			class ProgramBuilder;
			class ShaderDepository;

			class ComputeBuffer_OCL;
			class ComputeProgram_OCL;
			class ComputeController_OCL;

		}
	}
	
	


	namespace Util {
		typedef float Real;

		class Radian;
		class Degree;
		class Angle;
		class Math;

		class Ray;
		class Plane;
		class Sphere;
		class AxisAlignedBox;
	}
}
