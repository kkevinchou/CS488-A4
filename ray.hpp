#ifndef RAY_HPP
#define RAY_HPP

#include "algebra.hpp"
#include "background.hpp"

using namespace std;

class Ray {
public:
    Ray(const Point3D& pos, const Vector3D& dir, const Background& bg);
    Colour cast() const;

    Point3D pos;
    Vector3D dir;
    Background bg;
private:
};

#endif