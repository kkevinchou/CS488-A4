#include "raycaster.hpp"
#include <math.h>

extern bool debug;

RayCaster::RayCaster(const Point3D& eye, const Background& bg, const SceneNode *root, const list<Light *> &lights)
    : eye(eye), bg(bg), lights(lights), collider(root) {
}

cast_result RayCaster::cast(const Point3D &pos, const Vector3D &dir) const {
    list<collision_result> hits = collider.getCollisionData(pos, dir);

    cast_result castResult;
    castResult.hit = (hits.size() > 0) ? true : false;

    if (!castResult.hit) {
        return castResult;
    }

    // Find the closest hit

    collision_result closestHit;
    double minDistSq = INFINITY;

    for (list<collision_result>::iterator it = hits.begin(); it != hits.end(); it++) {
        double distSq = pos.distSq((*it).point);

        if (distSq < 0.01) {
            continue;
        }

        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestHit = (*it);
        }
    }

    if (minDistSq == INFINITY) {
        castResult.hit = false;
        return castResult;
    }

    castResult.colour = closestHit.colour;
    castResult.point = closestHit.point;
    castResult.dist = closestHit.point.dist(pos);
    castResult.id = closestHit.id;

    return castResult;
}

cast_result RayCaster::cast2(const Point3D &pos, const Vector3D &dir) const {
    cast_result primaryCast = cast(pos, dir);

    if (!primaryCast.hit) {
        return primaryCast;
    }

    bool inLight = false;

    cast_result secondaryCast;
    for (list<Light *>::const_iterator it = lights.begin(); it != lights.end(); it++) {
        secondaryCast = cast(primaryCast.point, (*it)->position - primaryCast.point);
        if (!secondaryCast.hit) {
            inLight = true;
            break;
        }
    }

    if (!inLight) {
        primaryCast.colour = Colour(0);
    }

    return primaryCast;
}