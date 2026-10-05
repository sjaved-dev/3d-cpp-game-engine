#pragma once
#include <vector>
#include <cstddef>

// A real object pool, used for its actual purpose here: recycling
// zombie and projectile slots each frame instead of constantly
// allocating and freeing memory, which is what the original project
// was about (avoiding heap churn during continuous spawning).
template <typename T, size_t PoolSize>
class ObjectPool {
private:
    struct Slot {
        T data;
        bool active = false;
    };
    std::vector<Slot> storage;

public:
    ObjectPool() : storage(PoolSize) {}

    // Returns a pointer to a free slot, or nullptr if the pool is full.
    T* acquire() {
        for (auto& slot : storage) {
            if (!slot.active) {
                slot.active = true;
                slot.data = T();
                return &slot.data;
            }
        }
        return nullptr;
    }

    void release(T* ptr) {
        for (auto& slot : storage) {
            if (&slot.data == ptr) {
                slot.active = false;
                return;
            }
        }
    }

    // Calls fn(T&) for every currently active element. Used each frame
    // to update and render only the entities that are actually alive.
    template <typename Fn>
    void forEachActive(Fn fn) {
        for (auto& slot : storage) {
            if (slot.active) fn(slot.data);
        }
    }

    size_t activeCount() const {
        size_t count = 0;
        for (auto& slot : storage) if (slot.active) count++;
        return count;
    }
};
