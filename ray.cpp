#include "ray.hpp"

Ray::Ray(const Point3D& pos, const Vector3D& dir, const Background& bg, const SceneNode *root, const Collider &collider)
    : pos(pos), dir(dir), bg(bg), root(root), collider(collider) {

}

Colour Ray::cast() const {
    collisiondata cData = collider.getCollisionData(pos, dir);
    return bg.getPixelColour(pos[0], pos[1]);
}