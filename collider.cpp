#include "collider.hpp"

Collider::Collider(const SceneNode *root) : root(root) {
}

collisiondata Collider::getCollisionData(const Point3D& pos, const Vector3D& dir) const {
    collisiondata d;
    return d;
}