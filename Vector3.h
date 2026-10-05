#pragma once
#include <cmath>
#include <iostream>

// A genuine 3D vector class with the core spatial math a first-person
// game actually needs: dot product (for angles/lighting-style checks),
// cross product (for facing/turning directions), and normalization.
struct Vector3 {
    float x, y, z;

    Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt(lengthSquared());
    }

    Vector3 normalized() const {
        float len = length();
        if (len < 1e-6f) return Vector3(0, 0, 0);
        return Vector3(x / len, y / len, z / len);
    }

    float distanceTo(const Vector3& other) const {
        return (*this - other).length();
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
};
