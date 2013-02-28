#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP

#include "algebra.hpp"
#include "background.hpp"
#include "scene.hpp"
#include "collider.hpp"

using namespace std;

struct ray {
    Point3D pos;
    Vector3D dir;
};

struct cast_result {
    bool hit;
    Colour colour;
};

class RayCaster {
public:
    RayCaster(const Point3D& eye, const Background& bg, const SceneNode *root);
    cast_result cast(const Point3D &pos, const Vector3D &dir) const;
private:
    const Point3D &eye;
    const Background &bg;
    Collider collider;
};

#endif