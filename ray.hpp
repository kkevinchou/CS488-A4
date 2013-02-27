#ifndef RAY_HPP
#define RAY_HPP

#include "algebra.hpp"
#include "background.hpp"
#include "scene.hpp"
#include "collider.hpp"

using namespace std;

class Ray {
public:
    Ray(const Point3D& pos, const Vector3D& dir, const Background& bg, const SceneNode *root, const Collider &collider);
    Colour cast() const;
private:
    Point3D pos;
    Vector3D dir;
    Background bg;
    const SceneNode *root;
    const Collider &collider;
};

#endif