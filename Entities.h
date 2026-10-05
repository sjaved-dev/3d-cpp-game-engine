#pragma once
#include "Vector3.h"

// Every entity in the simulation has a position and a collision radius.
// Real sphere-sphere collision detection (used in CollisionSystem.h)
// only needs these two things, which keeps this honest and simple
// rather than pretending to have a full physics engine.

struct Zombie {
    Vector3 position;
    float speed = 1.5f;
    float radius = 0.6f;
    int health = 30;
    bool alive() const { return health > 0; }
};

struct Projectile {
    Vector3 position;
    Vector3 velocity;
    float radius = 0.1f;
    float lifetime = 2.0f; // seconds remaining before it despawns
    int damage = 15;
};

struct Player {
    Vector3 position;
    Vector3 facing = Vector3(0, 0, 1); // forward direction, unit vector
    float radius = 0.5f;
    int health = 100;
    int score = 0;

    void turn(float yawRadians) {
        // Rotate the facing vector around the Y (up) axis.
        float cosA = std::cos(yawRadians);
        float sinA = std::sin(yawRadians);
        float newX = facing.x * cosA - facing.z * sinA;
        float newZ = facing.x * sinA + facing.z * cosA;
        facing = Vector3(newX, 0.0f, newZ).normalized();
    }
};
