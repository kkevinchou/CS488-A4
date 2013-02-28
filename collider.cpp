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
        Primitive *p = (*it)->getPrimitive();

        if (p != NULL) {
            switch (p->getType()) {
                case Primitive::NONHIERSPHERE:
                    NonhierSphere *s = static_cast<NonhierSphere *>(p);
                    result = nonhierSphereSolver(s, pos, dir);

                    if (!result.hit) {
                        continue;
                    } else {
                        return result;
                    }

                    break;
            }
        }
    }

    return result;
}

collision_result Collider::nonhierSphereSolver(NonhierSphere *s, const Point3D& pos, const Vector3D& dir) const {
    collision_result result;
    result.hit = false;

    double a = dir.dot(dir);
    double b = (pos - s->getPosition()).dot(dir) * 2;
    double c = (pos - s->getPosition()).dot((pos - s->getPosition())) - (s->getRadius() * s->getRadius());

    double roots[2];
    int quadResult = quadraticRoots(a, b, c, roots);

    if (quadResult > 0) {
        result.hit = true;
    }

    return result;
}