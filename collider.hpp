#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "scene.hpp"
#include <list>

using namespace std;

struct collision_result {
    Colour colour;
    Point3D point;
    Vector3D normal;
    int id;
    const PhongMaterial *phongMaterial;
};

class Collider {
public:
    Collider(const SceneNode *root);
    list<collision_result> getCollisionData(const Point3D& pos, const Vector3D& dir) const;
private:
    const SceneNode *root;

    list<collision_result> nonhierSphereSolver(NonhierSphere *p, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
};

#endif