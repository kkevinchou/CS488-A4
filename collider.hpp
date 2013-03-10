#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "scene.hpp"
#include "mesh.hpp"
#include "algebra.hpp"
#include <list>
#include "primitive.hpp"

using namespace std;

struct collision_result {
    Colour colour;
    Point3D point;
    Vector3D normal;
    const PhongMaterial *phongMaterial;
};

class Collider {
public:
    Collider(const SceneNode *root);
    list<collision_result> getCollisionData(const Point3D& pos, const Vector3D& dir) const;
    list<collision_result> getCollisionData2(const Point3D& pos, const Vector3D& dir) const;
    list<collision_result> getCollisionData2(const Point3D& pos, const Vector3D& dir, const SceneNode *node, Matrix4x4 trans, Matrix4x4 itrans) const;
private:
    const SceneNode *root;

    list<collision_result> nonhierSphereSolver(NonhierSphere *nhs, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
    list<collision_result> nonhierBoxSolver(NonhierBox *nhb, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
    list<collision_result> meshSolver(Mesh *mesh, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
    list<collision_result> sphereSolver(Sphere *s, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
    list<collision_result> cubeSolver(Cube *c, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const;
};

#endif