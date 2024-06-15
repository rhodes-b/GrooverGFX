
#ifndef _GROOVER_RAY3D_H
#define _GROOVER_RAY3D_H

#include "math3d.h"

struct Ray {
    struct Point3F32 start;
    struct Vec3 dir;

    struct Point3F32 (*point_at)(struct Ray* r, float t);
};

struct Ray make_ray(struct Point3F32 start, struct Vec3 dir);

/*

class Interval:

    """Simple representation of an open interval"""

    def __init__(self, low=0.0, high=math.inf):
        """
        >>> myInterval = Interval()
        >>> myInterval.low
        0.0
        >>> myInterval.high
        inf
        >>> myInterval = Interval(.001,200)
        >>> myInterval.low
        0.001
        >>> myInterval.high
        200.0
        """
        self.low, self.high = float(low), float(high)
       
    def __repr__(self):
        """
        >>> Interval()
        Interval(0.0, inf)
        >>> Interval(0.5, 200)
        Interval(0.5, 200.0)
        """
        return f"Interval({self.low}, {self.high})"

    def __contains__(self, t):
        """ returns True iff t is inside this interval (excluding endpoints)
        >>> inter = Interval(2,100)
        >>> 0 in inter
        False
        >>> 5 in inter
        True
        >>> 100 in inter
        False
        """
        return self.low < t < self.high

    def set(self, low, high):
        """ set endpoints of interval to low and high
        >>> inter = Interval()
        >>> inter.set(3.1, 4.5)
        >>> inter.low
        3.1
        >>> inter.high
        4.5
        """
        self.low, self.high = float(low), float(high)

*/

#endif /* _GROOVER_RAY3D_H */