#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "scene.hpp"

using namespace std;

struct collision_result {
    bool hit;
    Colour colour;
    Point3D point;
};

class Collider {
public:
    Collider(const SceneNode *root);
    collision_result getCollisionData(const Point3D& pos, const Vector3D& dir) const;
private:
    const SceneNode *root;

    collision_result nonhierSphereSolver(NonhierSphere *p, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
};

#endif