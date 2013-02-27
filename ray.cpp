#include "ray.hpp"

Ray::Ray(const Point3D& pos, const Vector3D& dir) : pos(pos), dir(dir) {

}

Colour Ray::cast() const {
    return Colour(0.5);
}