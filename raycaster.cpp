#include "raycaster.hpp"

RayCaster::RayCaster(const Point3D& eye, const Background& bg, const SceneNode *root)
    : eye(eye), bg(bg), collider(root) {
}

cast_result RayCaster::cast(const Point3D &pos, const Vector3D &dir) const {
    collision_result data = collider.getCollisionData(eye, dir);

    cast_result result;

    result.hit = data.hit;

    return result;
}