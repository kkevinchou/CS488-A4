#include "collider.hpp"

Collider::Collider(const SceneNode *root) : root(root) {
}

collision_result Collider::getCollisionData(const Point3D& pos, const Vector3D& dir) const {
    collision_result d;

    d.hit = false;

    return d;
}