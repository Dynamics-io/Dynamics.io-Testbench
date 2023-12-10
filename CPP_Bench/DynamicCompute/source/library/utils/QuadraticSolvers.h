#pragma once

#include "Math.h"


namespace DynamicCompute {
    namespace Util {
        namespace QuadraticSolvers {

            float QuadraticArcLength(float a, float b, float c, float i, float j);

            float QuadraticArcLengthEstimate(double a, double b, double c, double i, double j, int iterations);
        }
    }
}
        