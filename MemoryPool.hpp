/**
 * ================================================================================
 * DETERMINISTIC OBJECT POOL MANAGEMENT SYSTEM (CLEAN-ROOM IMPLEMENTATION)
 * ================================================================================
 * REGULATORY & COMPLIANCE DISCLAIMER:
 * This code acts as a functional architectural mockup representing low-level memory 
 * optimization systems developed for performance-constrained simulation configurations. 
 * This is an independent, original layout designed to showcase C++ syntax proficiency, 
 * object orientation logic, and memory allocation structures.
 * ================================================================================
 */

#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T, size_t PoolSize>
class DeterministicMemoryPool {
private:
    struct PoolObject {
        T data;
        bool is_active;
    };

    std::vector<PoolObject> m_storage_buffer;
    size_t m_active_allocations;

public:
    DeterministicMemoryPool() : m_active_allocations(0) {
        // Pre-allocate memory inside safe contiguous blocks to prevent heap allocation runtime spikes
        m_storage_buffer.resize(PoolSize);
        for (size_t i = 0; i < PoolSize; ++i) {
            m_storage_buffer[i].is_active = false;
        }
        std::cout << "Pre-allocated memory chunk for " << PoolSize << " synchronous items." << std::endl;
    }

    ~DeterministicMemoryPool() = default;

    T* acquire_element() {
        // Linear scan tracking for the next available pooled slot
        for (size_t i = 0; i < PoolSize; ++i) {
            if (!m_storage_buffer[i].is_active) {
                m_storage_buffer[i].is_active = true;
                m_active_allocations++;
                return &(m_storage_buffer[i].data);
            }
        }
        throw std::runtime_error("MemoryPool Exhaustion Error: Zero available block entries remain.");
    }

    void release_element(T* element_address) {
        for (size_t i = 0; i < PoolSize; ++i) {
            if (&(m_storage_buffer[i].data) == element_address) {
                if (m_storage_buffer[i].is_active) {
                    m_storage_buffer[i].is_active = false;
                    m_active_allocations--;
                    return;
                }
            }
        }
        std::cerr << "Memory Free Warning: Attempted to release unmanaged memory block." << std::endl;
    }

    size_t get_active_count() const { return m_active_allocations; }
};

#endif // MEMORY_POOL_HPP
