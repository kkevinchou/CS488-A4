#include "raycaster.hpp"
#include <math.h>

extern bool debug;

RayCaster::RayCaster(const Point3D& eye, const Background& bg, const SceneNode *root, const list<Light *> &lights, const Colour &ambient)
    : eye(eye), bg(bg), lights(lights), collider(root), ambient(ambient) {
}

cast_result RayCaster::cast(const Point3D &pos, const Vector3D &dir) const {
    list<collision_result> hits = collider.getCollisionData(pos, dir);

    cast_result castResult;
    castResult.hit = (hits.size() > 0) ? true : false;

    if (!castResult.hit) {
        return castResult;
    }

    // Find the closest hit

    collision_result closestHit;
    double minDistSq = INFINITY;

    for (list<collision_result>::iterator it = hits.begin(); it != hits.end(); it++) {
        double distSq = pos.distSq((*it).point);

        // We've collided with something too close, we were probably
        // casting from the surface and rehit the same object, so ignore
        if (distSq < 0.01) {
            continue;
        }

        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestHit = (*it);
        }
    }

    castResult.collisionResult = closestHit;
    if (minDistSq == INFINITY) {
        castResult.hit = false;
        return castResult;
    }

    castResult.collisionResult = closestHit;

    return castResult;
}

cast_result RayCaster::cast2(const Point3D &pos, const Vector3D &dir) const {
    cast_result primaryCast = cast(pos, dir);

    if (!primaryCast.hit) {
        return primaryCast;
    }

    primaryCast.finalColour = shade(primaryCast);

    return primaryCast;
}

Colour RayCaster::shade(struct cast_result primaryCast, const Light *light) const {
    Colour c(0);

    cast_result castResult;
    Point3D position = primaryCast.collisionResult.point;
    const PhongMaterial *phongMaterial = primaryCast.collisionResult.phongMaterial;

    castResult = cast(position, light->position - position);
    if (!castResult.hit) {
        double distSq = position.distSq(light->position);
        Vector3D lightVec = light->position - position;
        lightVec.normalize();
        Vector3D normal = primaryCast.collisionResult.normal;

        const double *falloff = light->falloff;
        double energyIn = lightVec.dot(normal);
        energyIn /= (falloff[0] + falloff[1] * sqrt(distSq) + falloff[2] * distSq);

        Vector3D r = (-1 * lightVec) + (2 * (lightVec.dot(normal)) * normal);
        Vector3D eyeVec = eye - position;
        eyeVec.normalize();

        Colour materialPropertiesColour = phongMaterial->get_diffuse() + (pow(r.dot(eyeVec), phongMaterial->get_shininess())) / normal.dot(lightVec) * phongMaterial->get_spec();

        c = light->colour * materialPropertiesColour * energyIn;
    }

    return c;
}

Colour RayCaster::shade(struct cast_result primaryCast) const {
    Colour finalColour = ambient * primaryCast.collisionResult.colour;;

    cast_result castResult;
    for (list<Light *>::const_iterator it = lights.begin(); it != lights.end(); it++) {
        Colour lightColour = shade(primaryCast, (*it));
        finalColour = finalColour + lightColour;
    }

    return finalColour;
}