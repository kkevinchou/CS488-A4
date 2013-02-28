#include "raycaster.hpp"

extern bool debug;

RayCaster::RayCaster(const Point3D& eye, const Background& bg, const SceneNode *root)
    : eye(eye), bg(bg), collider(root) {
}

cast_result RayCaster::cast(const Point3D &pos, const Vector3D &dir) const {
    list<collision_result> hits = collider.getCollisionData(pos, dir);

    cast_result castResult;
    castResult.hit = (hits.size() > 0) ? true : false;

    if (!castResult.hit) {
        return castResult;
    }

    // Find the closest hit

    collision_result closestHit = hits.front();
    double minDistSq = pos.distSq(closestHit.point);

    for (list<collision_result>::iterator it = hits.begin(); it != hits.end(); it++) {
        double distSq = pos.distSq((*it).point);
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestHit = (*it);
        }
    }

    castResult.colour = closestHit.colour;

    return castResult;
}