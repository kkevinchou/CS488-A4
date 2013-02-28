#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

using namespace std;

class Primitive {
public:
  enum Type {
    NONHIERSPHERE,
    NONHIERBOX,
    MESH,
    UNDEFINED
  };

  virtual ~Primitive();

  Primitive();

  virtual Type get_type() const {
    return Primitive::UNDEFINED;
  }

  static int nextId;
  int id;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  virtual ~Cube();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();

  Point3D get_position() const {
    return m_pos;
  }

  double get_radius() const {
    return m_radius;
  }

  virtual Type get_type() const {
    return Primitive::NONHIERSPHERE;
  }

private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }

  virtual Type get_type() const {
    return Primitive::NONHIERBOX;
  }

  virtual ~NonhierBox();

private:
  Point3D m_pos;
  double m_size;
};

#endif
