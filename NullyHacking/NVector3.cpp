#include "NVector3.h"

namespace Nully
{
  NVector3::NVector3()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
  {
  }

  NVector3::NVector3(const float a_x, const float a_y, const float a_z)
    : x(a_x)
    , y(a_y)
    , z(a_z)
  {
  }

  NVector3::NVector3(const NVector3 & a_vector1)
  {
    this->x = a_vector1.x;
    this->y = a_vector1.y;
    this->z = a_vector1.z;
  }
  NVector3 NVector3::Normalized() const
  {
    NVector3 tmp;
    float length = this->Length();

    tmp.x = this->x / length;
    tmp.y = this->y / length;
    tmp.z = this->z / length;

    return tmp;
  }
  void NVector3::Normalize()
  {
    float length = this->Length();

    this->x /= length;
    this->y /= length;
    this->z /= length;
  }
  float NVector3::Length() const
  {
    return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
  }

  float NVector3::GetAngleInRad() const
  {
    NVector3 normalized = this->Normalized();
    return atan2f(normalized.y, normalized.x);
  }

  float NVector3::Dot(const NVector3 & a_vector1, const NVector3 & a_vector2)
  {
    return ((a_vector1.x * a_vector2.x) + (a_vector1.y * a_vector2.y) + (a_vector1.z * a_vector2.z));
  }
  NVector3 NVector3::Cross(const NVector3 & a_vector1, const NVector3 & a_vector2)
  {
    NVector3 tmp;
    tmp.x = a_vector1.y * a_vector2.z - a_vector1.z*a_vector2.y;
    tmp.y = a_vector1.z * a_vector2.x - a_vector1.x*a_vector2.z;
    tmp.z = a_vector1.x * a_vector2.y - a_vector1.y*a_vector2.x;
    return tmp;
  }
  NVector3 NVector3::Reflect(const NVector3 & a_vector1, const NVector3 & a_vector2)
  {
    return NVector3::zero;
  }
  void NVector3::Normalize(NVector3 & a_vector1)
  {
    float length = a_vector1.Length();

    a_vector1.x /= length;
    a_vector1.y /= length;
    a_vector1.z /= length;
  }
  NVector3 NVector3::Normalized(const NVector3 & a_vector1)
  {
    NVector3 tmp;
    float length = a_vector1.Length();

    tmp.x = a_vector1.x / length;
    tmp.y /= a_vector1.y / length;
    tmp.z /= a_vector1.z / length;

    return tmp;
  }
  NVector3 NVector3::Project(const NVector3 & a_vector1, const NVector3 & a_vector2)
  {
    NVector3 tmp = (a_vector1 * a_vector2) / (a_vector2*a_vector2) * a_vector2;
    return tmp;
  }

  float NVector3::Distance(const NVector3 & a_vector1, const NVector3 & a_vector2)
  {
    auto tmp = a_vector1 - a_vector2;
    return tmp.Length();
  }

  NVector3 NVector3::zero = NVector3(0.0f, 0.0f, 0.0f);
  NVector3 NVector3::one = NVector3(1.0f, 1.0f, 1.0f);
  NVector3 NVector3::left = NVector3(-1.0f, 0.0f, 0.0f);
  NVector3 NVector3::right = NVector3(1.0f, 0.0f, 0.0f);
  NVector3 NVector3::up = NVector3(0.0f, 1.0f, 0.0f);
  NVector3 NVector3::down = NVector3(0.0f, -1.0f, 0.0f);
  NVector3 NVector3::back = NVector3(0.0f, 0.0f, -1.0f);
  NVector3 NVector3::forward = NVector3(0.0f, 0.0f, 1.0f);

  float NVector3::epsilon = 0.01235f;
}


