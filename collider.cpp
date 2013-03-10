#include <iostream>
#include <math.h>
#include "polyroots.hpp"
#include "collider.hpp"

Collider::Collider(const SceneNode *root) : root(root) {
}

list<collision_result> Collider::getCollisionData2(const Point3D& pos, const Vector3D& dir) const {
    Matrix4x4 trans;
    Matrix4x4 itrans;
    return getCollisionData2(pos, dir, root, trans, itrans);
}

list<collision_result> Collider::getCollisionData2(const Point3D& pos, const Vector3D& dir, const SceneNode* node, Matrix4x4 trans, Matrix4x4 itrans) const {
    list<collision_result> allHits;

    trans = node->get_transform() * trans;
    itrans = node->get_inverse() * itrans;

    Point3D tpos = itrans * pos;
    Vector3D tdir = itrans * dir;

    if (node->is_geometry()) {
        const GeometryNode *g = static_cast<const GeometryNode *>(node);
        list<collision_result> newHits;

        switch (g->get_type()) {
            case Primitive::NONHIERSPHERE:
                {
                    NonhierSphere *p = static_cast<NonhierSphere *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = nonhierSphereSolver(p, m, tpos, tdir);

                    break;
                }
            case Primitive::SPHERE:
                {
                    Sphere *p = static_cast<Sphere *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = sphereSolver(p, m, tpos, tdir);
                    break;
                }
            case Primitive::CUBE:
                {
                    Cube *c = static_cast<Cube *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = cubeSolver(c, m, tpos, tdir);
                    break;
                }
            case Primitive::NONHIERBOX:
                {
                    NonhierBox *p = static_cast<NonhierBox *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = nonhierBoxSolver(p, m, tpos, tdir);
                    break;
                }
            case Primitive::MESH:
                {
                    Mesh *p = static_cast<Mesh *>(g->get_primitive());
                    const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    newHits = meshSolver(p, m, tpos, tdir);
                    break;
                }
            default:
                // cerr << "unhandled primitive type" << endl;
                break;
        }

        allHits.insert(allHits.end(), newHits.begin(), newHits.end());
    }

    list<SceneNode *> children = node->get_children();
    for (list<SceneNode *>::const_iterator it = children.begin(); it != children.end(); ++it) {
        list<collision_result> childHits = getCollisionData2(pos, dir, *it, trans, itrans);
        allHits.insert(allHits.end(), childHits.begin(), childHits.end());
    }

    for (list<collision_result>::iterator it = allHits.begin(); it != allHits.end(); ++it) {
        it->point = node->get_transform() * it->point;
        it->normal = node->get_inverse().transpose() * it->normal;
        it->normal.normalize();
    }

    return allHits;
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
            case Primitive::NONHIERBOX:
                {
                    // NonhierBox *p = static_cast<NonhierBox *>(g->get_primitive());
                    // const PhongMaterial *m = static_cast<const PhongMaterial *>(g->get_material());
                    // newHits = meshSolver(p, m, pos, dir);
                    // break;
                }
        }

        allHits.insert(allHits.begin(), newHits.begin(), newHits.end());
    }

    return allHits;
}

list<collision_result> Collider::sphereSolver(Sphere *s, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    return nonhierSphereSolver(&s->m_nonhierSphere, m, pos, dir);
}

list<collision_result> Collider::nonhierSphereSolver(NonhierSphere *nhs, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    double a = dir.dot(dir);
    double b = (pos - nhs->get_position()).dot(dir) * 2;
    double c = (pos - nhs->get_position()).dot(pos - nhs->get_position()) - (nhs->get_radius() * nhs->get_radius());

    double roots[2];
    int quadResult = quadraticRoots(a, b, c, roots);

    list<collision_result> hits;
    for (int i = 0; i < quadResult; i++) {
        if (roots[i] < 0) continue;
        collision_result hit;
        hit.point = pos + (roots[i] * dir);
        hit.colour = m->get_diffuse();
        hit.normal = (hit.point - nhs->get_position());
        hit.normal.normalize();
        hit.phongMaterial = m;

        hits.push_back(hit);
    }

    return hits;
}

list<collision_result> Collider::cubeSolver(Cube *c, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    return nonhierBoxSolver(&c->m_nonhierBox, m, pos, dir);
}

list<collision_result> Collider::nonhierBoxSolver(NonhierBox *nhb, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    // vector<Point3D> verts;

    // Vector3D boxPosition = nhb->get_position();
    // double size = nhb->get_size();

    // verts.push_back(pos);
    // vers.push_back(pos + Vector3D(size, 0, 0));
    // vers.push_back(pos + Vector3D(size, size, 0));
    // vers.push_back(pos + Vector3D(0, size, 0));

    // verts.push_back(pos + Vector3D(0, 0, size));
    // vers.push_back(pos + Vector3D(size, 0, size));
    // vers.push_back(pos + Vector3D(size, size, size));
    // vers.push_back(pos + Vector3D(0, size, size));

    // vector<vector<int> > faces;
    // vector<int> face1 =

    Point3D boxPosition = nhb->get_position();
    double size = nhb->get_size();

    double tmin = -INFINITY, tmax = INFINITY;

    list<collision_result> hits;

    if (dir[0] == 0 || dir[1] == 0 || dir[2] == 0) {
        return hits;
    }

    double tx1 = (boxPosition[0] - pos[0]) / dir[0];
    double tx2 = (boxPosition[0] + size - pos[0]) / dir[0];

    tmin = max(tmin, min(tx1, tx2));
    tmax = min(tmax, max(tx1, tx2));

    double ty1 = (boxPosition[1] - pos[1]) / dir[1];
    double ty2 = (boxPosition[1] + size - pos[1]) / dir[1];

    tmin = max(tmin, min(ty1, ty2));
    tmax = min(tmax, max(ty1, ty2));

    double tz1 = (boxPosition[2] - pos[2]) / dir[2];
    double tz2 = (boxPosition[2] + size - pos[2]) / dir[2];

    tmin = max(tmin, min(tz1, tz2));
    tmax = min(tmax, max(tz1, tz2));

    // TODO: CLEANUP THE COLOUR CODE, LIFT UP INTO THE GETCOLLISIONDATA2 class
    // dont need to store colour, we can get from phong material

    if (tmax > tmin && tmin > 0) {
        collision_result hit;
        hit.point = pos + (tmin * dir);
        hit.colour = m->get_diffuse();
        hit.phongMaterial = m;

        if (abs(hit.point[0] - boxPosition[0]) < 0.01) {
            hit.normal = Vector3D(-1, 0, 0);
        } else if (abs(hit.point[0] - (boxPosition[0] + size)) < 0.01) {
            hit.normal = Vector3D(1, 0, 0);
        } else if (abs(hit.point[1] - boxPosition[1]) < 0.01) {
            hit.normal = Vector3D(0, -1, 0);
        } else if (abs(hit.point[1] - (boxPosition[1] + size)) < 0.01) {
            hit.normal = Vector3D(0, 1, 0);
        } else if (abs(hit.point[2] - boxPosition[2]) < 0.01) {
            hit.normal = Vector3D(0, 0, -1);
        } else if (abs(hit.point[2] - (boxPosition[2] + size)) < 0.01) {
            hit.normal = Vector3D(0, 0, 1);
        }

        hits.push_back(hit);
    }

    return hits;
}

list<collision_result> Collider::meshSolver(Mesh *mesh, const PhongMaterial *m, const Point3D& pos, const Vector3D& dir) const {
    list<collision_result> hits;
    vector<struct face_plane> facePlanes = mesh->facePlanes;

    for (unsigned int i = 0; i < facePlanes.size(); i++) {
        struct face_plane f = facePlanes[i];
        unsigned int numPoints = f.points.size();

        int refPointIndex = 1;
        Point3D refPoint = f.points[refPointIndex];
        Vector3D refNormal = (f.points[refPointIndex + 1] - refPoint).cross(f.points[refPointIndex - 1] - refPoint);
        refNormal.normalize();

        double t = (refPoint - pos).dot(refNormal) / (dir.dot(refNormal));
        if (t <= 0) continue;
        Point3D intersectPoint = pos + t * dir;

        collision_result hit;
        hit.point = intersectPoint;
        hit.normal = refNormal;

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
            hit.phongMaterial = m;
            hits.push_back(hit);
        }
    }

    return hits;
}