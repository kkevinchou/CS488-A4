#include <iostream>
#include "polyroots.hpp"
#include "collider.hpp"

Collider::Collider(const SceneNode *root) : root(root) {
}

list<collision_result> Collider::getCollisionData(const Point3D& pos, const Vector3D& dir) const {
    list<collision_result> allHits;

    list<SceneNode *> objs = root->get_children();

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
            case Primitive::MESH:
                {
                    Mesh *p = static_cast<Mesh *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = meshSolver(p, m, pos, dir);
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
        hit.phongMaterial = m;

        hits.push_back(hit);
    }

    return hits;
}

bool debug = true;

list<collision_result> Collider::meshSolver(Mesh *mesh, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    list<collision_result> hits;
    vector<struct face_plane> facePlanes = mesh->facePlanes;

    for (unsigned int i = 0; i < facePlanes.size(); i++) {
        struct face_plane f = facePlanes[i];

        unsigned int numPoints = f.points.size();

        collision_result hit;

        int refPointIndex = 1;
        Point3D refPoint = f.points[refPointIndex];
        Vector3D refNormal = (f.points[refPointIndex + 1] - refPoint).cross(f.points[refPointIndex - 1] - refPoint);
        refNormal.normalize();

        double t = (refPoint - pos).dot(refNormal) / (dir.dot(refNormal));
        if (t <= 0) continue;
        Point3D intersectPoint = pos + t * dir;

        hit.point = intersectPoint;
        hit.normal = refNormal;

        if (debug) {
            // cerr << hit.normal << endl;
            debug = false;
        }

        bool passesPoints = true;
        for (unsigned int j = 0; j < numPoints; j++) {
            unsigned int nextIndex = (j + 1) % numPoints;

            Point3D point = f.points[j];
            Vector3D windingVec = f.points[nextIndex] - point;

            Vector3D refVector = refNormal.cross(windingVec);
            refVector.normalize();
            if ((intersectPoint - point).dot(refVector) <= 0) {
                passesPoints = false;
                break;
            }
        }

        if (passesPoints) {
            hit.colour = m->get_diffuse();
            hit.id = mesh->id;
            hit.phongMaterial = m;
            hits.push_back(hit);
        }
    }

    return hits;
}