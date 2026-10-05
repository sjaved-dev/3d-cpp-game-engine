# 3D First-Person Simulator & Game Engine Core
**BSc Computer Science Capstone Project | University of Management and Technology**  
*Awarded Grade: A / A- | High-Performance Native Systems Showcase*

---

## 📋 Project Overview
This repository contains the architecture, core algorithmic implementations, and technical documentation for a modular, low-level 3D simulation environment and engine built from the ground up using C++17 and Object-Oriented Programming (OOP). 

The engine was designed to solve a specific engineering constraint: maximizing real-time performance, stable frame rendering, and deterministic thread/memory behavior on resource-constrained hardware profiles without relying on heavy third-party runtime frameworks like Unity or Unreal.

---

## ⚙️ Engine Sub-System Implementations & Technical Choices
* **Dynamic Object Pooling (`ObjectPool.h`):** To prevent runtime heap fragmentation and garbage collection/deallocation latency spikes during continuous object instantiation (e.g., procedural entities, projectiles, active threats), a generic memory management system pre-allocates entity records in contiguous memory blocks and recycles inactive pointers to maintain highly stable frame times.
* **Hand-Coded Spatial Mathematics & Collision (`Vector3.h`, `Collision.h`):** Custom 3D vector transformations, dot/cross products, normalization, and distance calculations built without external math packages. Collision detection utilizes deterministic sphere-sphere collision checking routines.
* **Decoupled Execution Architecture & Entities (`Entities.h`, `main.cpp`):** The engine isolates entity update cycles, physics calculations, and behavioral routines onto clean, predictable execution loops to ensure system bottlenecks do not stall main simulation updates.

---

## 📊 Repository Scope & Implementation Note
To ensure complete zero-dependency portability across standard terminal environments, headless builds, and online evaluation compilers without requiring complex native graphics drivers (e.g., OpenGL/DirectX), this repository provides the full compiled backend systems rendered through a real-time top-down ASCII map interface in `main.cpp`. All underlying mathematical, memory management, and collision mechanics remain fully active in real time.

---

## 🧠 Relevance to Agentic AI Systems & Continuous Monitoring
Building simulation engines at this low level requires mastery of linear algebra, memory tracing, and system anomaly management. This mathematical and programmatic foundation maps onto the technical requirements of developing complex distributed simulation loops, performance-oriented tracking tools, and deterministic environments for autonomous learning nodes.

---

## 🛠️ How to Build and Run Locally

### Requirements
* A C++17 compliant compiler (`g++`, `clang++`, or MSVC).

### Terminal Command Line
```bash
g++ -std=c++17 -Wall -Wextra -o engine_sim main.cpp
./engine_sim
