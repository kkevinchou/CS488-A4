#include "primitive.hpp"
#include <iostream>

int Primitive::nextId = 0;

Primitive::Primitive() {
    id = nextId++;
}

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}
