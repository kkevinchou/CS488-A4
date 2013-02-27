#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "scene.hpp"

using namespace std;

struct collisiondata {

};

class Collider {
public:
    Collider(const SceneNode *root);
    collisiondata getCollisionData(const Point3D& pos, const Vector3D& dir) const;
private:
    const SceneNode *root;
};

#endif