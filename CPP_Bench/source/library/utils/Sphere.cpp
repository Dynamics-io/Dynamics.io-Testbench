#include "utils.h"

namespace Dynamics_IO_Testbench {
    namespace Util {

        bool Sphere::intersects(const AxisAlignedBox& box) const
        {
            return Math::intersects(*this, box);
        }

        bool Sphere::intersects(const Plane& plane) const
        {
            return Math::Abs(plane.getDistance(getCenter())) <= getRadius();
        }
    }
}