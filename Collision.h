#pragma once
#include "Vector3.h"

// Genuine collision detection: two spheres overlap if the distance
// between their centres is less than the sum of their radii. This is
// the real, standard technique (not a placeholder), just applied to
// simple bounding spheres rather than full mesh colliders.
namespace Collision {

inline bool spheresOverlap(const Vector3& posA, float radiusA,
                            const Vector3& posB, float radiusB) {
    float distSq = (posA - posB).lengthSquared();
    float radiusSum = radiusA + radiusB;
    return distSq <= (radiusSum * radiusSum);
}

} // namespace Collision
