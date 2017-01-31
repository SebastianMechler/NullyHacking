#pragma once
#include <cmath>

#include "NMath.h"

namespace Nully
{
  class NVector3
  {
  public:
    ////=======================================================================
    //// Constructor / Destructor
    ////=======================================================================

    /// @brief Default Constructor.
    NVector3();

    /// @brief Constructor with arguments.
    /// @param a_x float value which will initialize the x-component of vector.
    /// @param a_y float value which will initialize the y-component of vector.
    /// @param a_z float value which will initialize the z-component of vector.
    NVector3(const float a_x, const float a_y, const float a_z);

    /// @brief Copy Constructor.
    /// @param a_vector1 Vector which will initialize the vector.
    NVector3(const NVector3& a_vector1);

    ////=======================================================================
    //// Public Member functions
    ////=======================================================================

    /// @brief Returns the vector with normalization.
    /// @return Returns the vector with length = 1.0f.
    NVector3 Normalized() const;

    /// @brief Normalizes the vector.
    void Normalize();

    /// @brief Calculates the length (magnitude) of vector.
    /// @return Returns the length of the vector
    float Length() const;

    /// @brief Calculates the angle of the vector in normalized form (direction)
    /// @return Returns the angle in radians (look direction of vector)
    float GetAngleInRad() const;

    ////=======================================================================
    //// Operator overload
    ////=======================================================================

    /// @brief Assigns a vector.
    /// @param a_vector1 Vector which will be assigned.
    inline void operator=(const NVector3& a_vector1);

    /// @brief Adds a vector.
    /// @param a_vector1 Vector which will be added.
    inline void operator+=(const NVector3& a_vector1);

    /// @brief Subtracts a vector.
    /// @param a_vector1 Vector which will be subtracted.
    inline void operator-=(const NVector3& a_vector1);

    /// @brief Multiplies a vector.
    /// @param a_vector1 Vector which will be multiplied.
    inline void operator*=(const NVector3& a_vector1);

    /// @brief Adds a scalar.
    /// @param a_scalar Scalar which will be multiplied.
    inline void operator*=(const float a_scalar);

    ////=======================================================================
    //// Public Member variables
    ////=======================================================================

    /// @brief Components of the vector
    union
    {
      struct
      {
        float x;
        float y;
        float z;
      };
      float components[3];
    };

    ////=======================================================================
    //// Static Functions
    ////=======================================================================

    /// @brief Calculates the dot product of two vectors.
    /// @param a_vector1 Requires a normalized Vector.
    /// @param a_vector2 Requires a normalized Vector.
    /// @return Returns a float value which ranges from -1.0f to 1.0f depending on the vector direction.
    static float Dot(const NVector3& a_vector1, const NVector3& a_vector2);

    /// @brief Calculates the cross product of two vectors.
    /// @param a_vector1 Requires a normalized Vector.
    /// @param a_vector2 Requires a normalized Vector.
    /// @return Returns a vector which will be orthogonal to both vectors.
    static NVector3 Cross(const NVector3& a_vector1, const NVector3& a_vector2);

    /// @brief NOT IMPLEMENTED YET
    static NVector3 Reflect(const NVector3& a_vector1, const NVector3& a_vector2);

    /// @brief Normalizes a vector. The length will be set to 1.0f.
    /// @param a_vector1 A vector as reference.
    static void Normalize(NVector3& a_vector1);

    /// @brief Generates a normalized vector from another vector.
    /// @param a_vector1 A vector as reference.
    /// @return Returns a normalized vector.
    static NVector3 Normalized(const NVector3& a_vector1);

    /// @brief Projects a vector onto another vector.
    /// @param a_vector1 A vector as reference.
    /// @param a_vector2 A vector as reference.
    /// @return Returns a projected vector.
    static NVector3 Project(const NVector3& a_vector1, const NVector3& a_vector2);

    /// @brief Calculates the distance between two vectors.
    /// @param a_vector1 A vector as reference.
    /// @param a_vector2 A vector as reference.
    /// @return Returns the distance between the two passed vectors.
    static float Distance(const NVector3& a_vector1, const NVector3& a_vector2);

    ////=======================================================================
    //// Static Members
    ////=======================================================================

    /// @brief Static Unit Vectors
    static NVector3 zero;
    static NVector3 one;
    static NVector3 left;
    static NVector3 right;
    static NVector3 up;
    static NVector3 down;
    static NVector3 back;
    static NVector3 forward;

    /// @brief Static floating value required for calculating equalization from vectors.
    static float epsilon;
  };
}

////=======================================================================
//// Global Operator overload
////=======================================================================

inline void Nully::NVector3::operator=(const NVector3 & a_vector1)
{
  this->x = a_vector1.x;
  this->y = a_vector1.y;
  this->z = a_vector1.z;
}
inline void Nully::NVector3::operator+=(const NVector3 & a_vector1)
{
  this->x += a_vector1.x;
  this->y += a_vector1.y;
  this->z += a_vector1.z;
}
inline void Nully::NVector3::operator-=(const NVector3 & a_vector1)
{
  this->x -= a_vector1.x;
  this->y -= a_vector1.y;
  this->z -= a_vector1.z;
}
inline void Nully::NVector3::operator*=(const NVector3 & a_vector1)
{
  this->x *= a_vector1.x;
  this->y *= a_vector1.y;
  this->z *= a_vector1.z;
}
inline void Nully::NVector3::operator*=(const float a_scalar)
{
  this->x *= a_scalar;
  this->y *= a_scalar;
  this->z *= a_scalar;
}

/// @brief Adds two vectors.
/// @param a_vector1 A vector as reference.
/// @param a_vector2 A vector as reference.
/// @return Returns the sum of both vectors.
inline Nully::NVector3 operator+(const Nully::NVector3& a_vector1, const Nully::NVector3& a_vector2)
{
  Nully::NVector3 tmp;
  tmp.x = a_vector1.x + a_vector2.x;
  tmp.y = a_vector1.y + a_vector2.y;
  tmp.z = a_vector1.z + a_vector2.z;
  return tmp;
}

/// @brief Subtracts two vectors.
/// @param a_vector1 A vector as reference.
/// @param a_vector2 A vector as reference.
/// @return Returns the subtraction of both vectors.
inline Nully::NVector3 operator-(const Nully::NVector3& a_vector1, const Nully::NVector3& a_vector2)
{
  Nully::NVector3 tmp;
  tmp.x = a_vector1.x - a_vector2.x;
  tmp.y = a_vector1.y - a_vector2.y;
  tmp.z = a_vector1.z - a_vector2.z;
  return tmp;
}

/// @brief Multiplies two vectors.
/// @param a_vector1 A vector as reference.
/// @param a_vector2 A vector as reference.
/// @return Returns the multiplication of both vectors.
inline Nully::NVector3 operator*(const Nully::NVector3& a_vector1, const Nully::NVector3& a_vector2)
{
  Nully::NVector3 tmp;
  tmp.x = a_vector1.x * a_vector2.x;
  tmp.y = a_vector1.y * a_vector2.y;
  tmp.z = a_vector1.z * a_vector2.z;
  return tmp;
}

/// @brief Scales a vector.
/// @param a_vector1 A vector as reference.
/// @param a_scalar A scalar which will be multiplied with the vector.
/// @return Returns a new sized vector.
inline Nully::NVector3 operator*(const Nully::NVector3& a_vector1, const float a_scalar)
{
  Nully::NVector3 tmp;
  tmp.x = a_vector1.x * a_scalar;
  tmp.y = a_vector1.y * a_scalar;
  tmp.z = a_vector1.z * a_scalar;
  return tmp;
}

/// @brief Scales a vector.
/// @param a_scalar A scalar which will be multiplied with the vector.
/// @param a_vector1 A vector as reference.
/// @return Returns a new sized vector.
inline Nully::NVector3 operator*(const float a_scalar, const Nully::NVector3& a_vector1)
{
  Nully::NVector3 tmp;
  tmp.x = a_vector1.x * a_scalar;
  tmp.y = a_vector1.y * a_scalar;
  tmp.z = a_vector1.z * a_scalar;
  return tmp;
}

/// @brief Divides a vector.
/// @param a_vector1 A vector as reference.
/// @param a_vector2 A vector as reference.
/// @return Returns the division of both vectors.
inline Nully::NVector3 operator/(const Nully::NVector3& a_vector1, const Nully::NVector3& a_vector2)
{
  Nully::NVector3 tmp;
  tmp.x = a_vector1.x / a_vector2.x;
  tmp.y = a_vector1.y / a_vector2.y;
  tmp.z = a_vector1.z / a_vector2.z;
  return tmp;
}

/// @brief Checks for equalisation of two vectors.
/// @param a_vector1 A vector as reference.
/// @param a_vector2 A vector as reference.
/// @return Returns true if they are equal, else false.
inline bool operator==(const Nully::NVector3& a_vector1, const Nully::NVector3& a_vector2)
{
  return (abs(abs(a_vector1.x) - abs(a_vector2.x)) < Nully::NVector3::epsilon) &&
    (abs(abs(a_vector1.y) - abs(a_vector2.y)) < Nully::NVector3::epsilon) &&
    (abs(abs(a_vector1.z) - abs(a_vector2.z)) < Nully::NVector3::epsilon);
}

/// @brief Checks for non-equalisation of two vectors.
/// @param a_vector1 A vector as reference.
/// @param a_vector2 A vector as reference.
/// @return Returns true if they are not equal, else false.
inline bool operator!=(const Nully::NVector3& a_vector1, const Nully::NVector3& a_vector2)
{
  return !(a_vector1 == a_vector2);
}
