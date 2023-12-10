#include "utils/QuadraticSolvers.h"
#include "Math.h"

using namespace DynamicCompute::Util;


float QuadraticSolvers::QuadraticArcLength(float a, float b, float c, float i, float j) {
    float aj = a * j;
    float ai = a * i;
    float _2aj = 2 * aj;
    float _2ai = 2 * ai;

    float sqrt_1_2aj_b_2 = sqrtf(1 + powf(_2aj + b, 2));
    float sqrt_1_2ai_b_2 = sqrtf(1 + powf(_2ai + b, 2));

    float s1 = 2 * a * j * sqrt_1_2aj_b_2;
    float s2 = 2 * a * i * sqrt_1_2ai_b_2;
    float s3 = b * sqrt_1_2aj_b_2;
    float s4 = b * sqrt_1_2ai_b_2;
    float s5 = logf(_2aj + b + sqrt_1_2aj_b_2);
    float s6 = logf(_2ai + b + sqrt_1_2ai_b_2);

    return (float)((s1 - s2 + s3 - s4 + s5 - s6) / (4 * a));

}

float DynamicCompute::Util::QuadraticSolvers::QuadraticArcLengthEstimate(double a, double b, double c, double i, double j, int iterations)
{
    double sum_res = 0;

    double x_step = 1.0f / iterations;
    double x_step_half = x_step / 2.0f;
    double x = i;
    for (int it = 0; it <= iterations; it++)
    {
        double x1 = x + x_step_half;
        double x2 = x1 + x_step;

        double y1 = pow(a * x1, 2) + (b * x1) + c;
        double y2 = pow(a * x2, 2) + (b * x2) + c;

        sum_res += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

        if (it == iterations - 1) {
            printf("Ended on: %f, %f\n", x1, x2);
        }

        x += x_step;
    }

    return sum_res;
}
