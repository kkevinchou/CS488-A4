#include <iostream>
#include "polyroots.hpp"
#include "collider.hpp"

Collider::Collider(const SceneNode *root) : root(root) {
}

collision_result Collider::getCollisionData(const Point3D& pos, const Vector3D& dir) const {
    collision_result result;
    result.hit = false;

    list<SceneNode *> objs = root->get_children();

    int counter = 0;
    for (list<SceneNode *>::iterator it = objs.begin(); it != objs.end(); it++) {
        if (!(*it)->is_geometry()) {
            continue;
        }

        GeometryNode *g = static_cast<GeometryNode *>(*it);

        switch (g->get_type()) {
            case Primitive::NONHIERSPHERE:
                {
                    NonhierSphere *p = static_cast<NonhierSphere *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());

                    result = nonhierSphereSolver(p, m, pos, dir);

                    if (!result.hit) {
                        continue;
                    } else {
                        return result;
                    }

                    break;
                }
            case Primitive::UNDEFINED:
                {
                    cerr << "UNDEFINED PRIMITIVE TYPE" << endl;
                    break;
                }
        }
    }

    return result;
}

collision_result Collider::nonhierSphereSolver(NonhierSphere *p, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    collision_result result;
    result.hit = false;

    double a = dir.dot(dir);
    double b = (pos - p->getPosition()).dot(dir) * 2;
    double c = (pos - p->getPosition()).dot((pos - p->getPosition())) - (p->getRadius() * p->getRadius());

    double roots[2];
    int quadResult = quadraticRoots(a, b, c, roots);

    if (quadResult > 0) {
        result.hit = true;
        result.colour = m->get_diffuse();
    }

    return result;
}