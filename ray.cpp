#include "ray.hpp"

Ray::Ray(const Point3D& pos, const Vector3D& dir, const Background& bg) : pos(pos), dir(dir), bg(bg) {

}

Colour Ray::cast() const {
    return bg.getPixelColour(pos[0], pos[1]);
}