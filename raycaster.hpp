#ifndef RAYCASTER_HPP
#define RAYCASTER_HPP

#include "algebra.hpp"
#include "background.hpp"
#include "scene.hpp"
#include "collider.hpp"
#include "light.hpp"

using namespace std;

struct ray {
    Point3D pos;
    Vector3D dir;
};

struct cast_result {
    bool hit;
    int id;
    Colour colour;
    Point3D point;
    double dist;
};

class RayCaster {
public:
    RayCaster(const Point3D& eye, const Background& bg, const SceneNode *root, const list<Light *> &lights);
    cast_result cast(const Point3D &pos, const Vector3D &dir) const;
    cast_result cast2(const Point3D &pos, const Vector3D &dir) const;
private:
    const Point3D &eye;
    const Background &bg;
    const list<Light *> &lights;
    Collider collider;
};

#endif