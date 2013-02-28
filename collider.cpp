#include <iostream>
#include "polyroots.hpp"
#include "collider.hpp"

Collider::Collider(const SceneNode *root) : root(root) {
}

list<collision_result> Collider::getCollisionData(const Point3D& pos, const Vector3D& dir) const {
    list<collision_result> allHits;

    list<SceneNode *> objs = root->get_children();

    int counter = 0;
    for (list<SceneNode *>::iterator it = objs.begin(); it != objs.end(); it++) {
        if (!(*it)->is_geometry()) {
            continue;
        }

        GeometryNode *g = static_cast<GeometryNode *>(*it);
        list<collision_result> newHits;

        switch (g->get_type()) {
            case Primitive::NONHIERSPHERE:
                {
                    NonhierSphere *p = static_cast<NonhierSphere *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = nonhierSphereSolver(p, m, pos, dir);

                    break;
                }
            case Primitive::UNDEFINED:
                {
                    cerr << "UNDEFINED PRIMITIVE TYPE" << endl;
                    break;
                }
        }

        allHits.insert(allHits.begin(), newHits.begin(), newHits.end());
    }

    return allHits;
}

list<collision_result> Collider::nonhierSphereSolver(NonhierSphere *p, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    double a = dir.dot(dir);
    double b = (pos - p->get_position()).dot(dir) * 2;
    double c = (pos - p->get_position()).dot((pos - p->get_position())) - (p->get_radius() * p->get_radius());

    double roots[2];
    int quadResult = quadraticRoots(a, b, c, roots);

    list<collision_result> hits;
    for (int i = 0; i < quadResult; i++) {
        if (roots[i] < 0) continue;
        collision_result hit;
        hit.point = pos + (roots[i] * dir);
        hit.colour = m->get_diffuse();
        hit.normal = (hit.point - p->get_position());
        hit.normal.normalize();
        hit.id = p->id;

        hits.push_back(hit);
    }

    return hits;
}