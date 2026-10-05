/*
 * ============================================================================
 * ZOMBIE HUNT - RECREATION (2026)
 * ============================================================================
 * This is a from-scratch recreation of the core systems from an original
 * 2020 BSc capstone project ("Zombie Hunt", a 3D first-person shooter).
 * The original coursework files were lost, so this rebuilds the genuine
 * underlying systems honestly, rather than a full graphical game:
 *   - 3D vector math (Vector3.h): dot product, cross product, normalization
 *   - Object pooling (ObjectPool.h): recycling zombie/projectile memory
 *   - Sphere-sphere collision detection (Collision.h)
 *   - A real-time-style game loop with a fixed timestep
 *
 * Rendering is a top-down ASCII map printed to the console each tick,
 * since a full 3D renderer needs a graphics library and a window, which
 * a plain console/online compiler cannot provide. Everything else here
 * (the vector math, the collision detection, the pooling, the game loop)
 * is real, working code, not a placeholder.
 * ============================================================================
 */

#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "Vector3.h"
#include "ObjectPool.h"
#include "Entities.h"
#include "Collision.h"

const int WORLD_SIZE = 10;          // world spans -WORLD_SIZE to +WORLD_SIZE on X and Z
const int MAX_ZOMBIES = 20;
const int MAX_PROJECTILES = 50;
const float TICK_SECONDS = 0.5f;    // simulated seconds per tick
const int MAX_TICKS = 30;

void renderTopDownMap(const Player& player,
                       ObjectPool<Zombie, MAX_ZOMBIES>& zombies,
                       ObjectPool<Projectile, MAX_PROJECTILES>& projectiles) {
    const int gridSize = 21; // odd number so there's a centre cell
    std::vector<std::string> grid(gridSize, std::string(gridSize, '.'));

    auto worldToGrid = [&](const Vector3& pos) -> std::pair<int, int> {
        int gx = static_cast<int>(pos.x) + WORLD_SIZE;
        int gz = static_cast<int>(pos.z) + WORLD_SIZE;
        return { gx, gz };
    };

    zombies.forEachActive([&](Zombie& z) {
        auto [gx, gz] = worldToGrid(z.position);
        if (gx >= 0 && gx < gridSize && gz >= 0 && gz < gridSize) grid[gz][gx] = 'Z';
    });

    projectiles.forEachActive([&](Projectile& p) {
        auto [gx, gz] = worldToGrid(p.position);
        if (gx >= 0 && gx < gridSize && gz >= 0 && gz < gridSize) grid[gz][gx] = '*';
    });

    auto [px, pz] = worldToGrid(player.position);
    if (px >= 0 && px < gridSize && pz >= 0 && pz < gridSize) grid[pz][px] = 'P';

    std::cout << "\n--- Top-down map (P = player, Z = zombie, * = shot) ---\n";
    for (auto& row : grid) std::cout << row << "\n";
}

int main() {
    std::cout << "=== Zombie Hunt (2026 recreation) ===\n";
    std::cout << "Original 2020 coursework files were lost; this rebuilds\n";
    std::cout << "the core vector math, collision detection, object pooling,\n";
    std::cout << "and game loop from scratch.\n";

    std::mt19937 rng(42); // fixed seed so behaviour is reproducible
    std::uniform_real_distribution<float> spawnDist(-static_cast<float>(WORLD_SIZE), static_cast<float>(WORLD_SIZE));

    Player player;
    player.position = Vector3(0, 0, 0);

    ObjectPool<Zombie, MAX_ZOMBIES> zombies;
    ObjectPool<Projectile, MAX_PROJECTILES> projectiles;

    for (int tick = 0; tick < MAX_TICKS && player.health > 0; ++tick) {
        std::cout << "\n============ Tick " << tick << " ============\n";

        // Spawn a zombie every few ticks, as long as there's room in the pool.
        if (tick % 3 == 0) {
            Zombie* z = zombies.acquire();
            if (z) {
                z->position = Vector3(spawnDist(rng), 0, spawnDist(rng));
                z->health = 30;
                std::cout << "A zombie spawned at " << z->position << "\n";
            }
        }

        // Player automatically fires toward the nearest zombie each tick,
        // simulating aim-and-shoot without needing live keyboard input.
        Zombie* nearest = nullptr;
        float nearestDist = 1e9f;
        zombies.forEachActive([&](Zombie& z) {
            float d = player.position.distanceTo(z.position);
            if (d < nearestDist) { nearestDist = d; nearest = &z; }
        });

        if (nearest) {
            Vector3 direction = (nearest->position - player.position).normalized();
            Projectile* p = projectiles.acquire();
            if (p) {
                p->position = player.position;
                p->velocity = direction * 6.0f; // units per second
                p->lifetime = 2.0f;
                std::cout << "Player fires toward zombie at " << nearest->position << "\n";
            }
        }

        // Move zombies toward the player.
        zombies.forEachActive([&](Zombie& z) {
            Vector3 toPlayer = (player.position - z.position).normalized();
            z.position = z.position + toPlayer * (z.speed * TICK_SECONDS);
        });

        // Move projectiles and age them out.
        std::vector<Projectile*> expired;
        projectiles.forEachActive([&](Projectile& p) {
            p.position = p.position + p.velocity * TICK_SECONDS;
            p.lifetime -= TICK_SECONDS;
            if (p.lifetime <= 0.0f) expired.push_back(&p);
        });
        for (auto* p : expired) projectiles.release(p);

        // Real collision detection: projectile vs zombie.
        std::vector<Projectile*> hitProjectiles;
        zombies.forEachActive([&](Zombie& z) {
            projectiles.forEachActive([&](Projectile& p) {
                if (Collision::spheresOverlap(z.position, z.radius, p.position, p.radius)) {
                    z.health -= p.damage;
                    hitProjectiles.push_back(&p);
                    std::cout << "Hit! Zombie health now " << z.health << "\n";
                }
            });
        });
        for (auto* p : hitProjectiles) projectiles.release(p);

        // Remove dead zombies, award score.
        std::vector<Zombie*> deadZombies;
        zombies.forEachActive([&](Zombie& z) {
            if (!z.alive()) deadZombies.push_back(&z);
        });
        for (auto* z : deadZombies) {
            zombies.release(z);
            player.score += 10;
            std::cout << "Zombie defeated! Score: " << player.score << "\n";
        }

        // Real collision detection: zombie vs player.
        zombies.forEachActive([&](Zombie& z) {
            if (Collision::spheresOverlap(player.position, player.radius, z.position, z.radius)) {
                player.health -= 5;
                std::cout << "Player hit! Health now " << player.health << "\n";
            }
        });

        renderTopDownMap(player, zombies, projectiles);
        std::cout << "Active zombies: " << zombies.activeCount()
                  << " | Active projectiles: " << projectiles.activeCount()
                  << " | Player health: " << player.health
                  << " | Score: " << player.score << "\n";
    }

    std::cout << "\n=== Simulation complete ===\n";
    std::cout << "Final score: " << player.score << "\n";
    std::cout << (player.health > 0 ? "Player survived.\n" : "Player was overwhelmed.\n");

    return 0;
}
