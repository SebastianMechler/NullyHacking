#include "NVector4.h"

namespace Nully
{
  NVector4::NVector4()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , w(0.0f)
  {
  }
  NVector4::NVector4(const float a_x, const float a_y, const float a_z, const float a_w)
    : x(a_x)
    , y(a_y)
    , z(a_z)
    , w(a_w)
  {
  }

  NVector4::NVector4(const NVector4 & a_vector1)
  {
    this->x = a_vector1.x;
    this->y = a_vector1.y;
    this->z = a_vector1.z;
    this->w = a_vector1.w;
  }
  NVector4 NVector4::Normalized() const
  {
    NVector4 tmp;
    float length = this->Length();

    tmp.x = this->x / length;
    tmp.y = this->y / length;
    tmp.z = this->z / length;
    tmp.w = this->w / length;

    return tmp;
  }
  void NVector4::Normalize()
  {
    float length = this->Length();

    this->x /= length;
    this->y /= length;
    this->z /= length;
    this->w /= length;
  }
  float NVector4::Length() const
  {
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
  }

  float NVector4::Dot(const NVector4 & a_vector1, const NVector4 & a_vector2)
  {
    return ((a_vector1.x * a_vector2.x) + (a_vector1.y * a_vector2.y) + (a_vector1.z * a_vector2.z) + (a_vector1.w * a_vector2.w));
  }
  void NVector4::Normalize(NVector4 & a_vector1)
  {
    float length = a_vector1.Length();

    a_vector1.x /= length;
    a_vector1.y /= length;
    a_vector1.z /= length;
    a_vector1.w /= length;
  }
  NVector4 NVector4::Normalized(const NVector4 & a_vector1)
  {
    NVector4 tmp;
    float length = a_vector1.Length();

    tmp.x = a_vector1.x / length;
    tmp.y /= a_vector1.y / length;
    tmp.z /= a_vector1.z / length;
    tmp.w /= a_vector1.w / length;

    return tmp;
  }
  float NVector4::Distance(const NVector4 & a_vector1, const NVector4 & a_vector2)
  {
    auto tmp = a_vector1 - a_vector2;
    return tmp.Length();
  }
}