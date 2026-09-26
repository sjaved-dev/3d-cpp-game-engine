# 3D First-Person Simulator & Game Engine from Scratch
**BSc Computer Science Capstone Project | University of Management and Technology**  
*Awarded Grade: A / A- | High-Performance Native Systems Showcase*

## 📋 Project Overview
This repository contains the architecture and technical documentation for a modular, low-level 3D simulation environment and rendering engine built completely from the ground up using native C++ and pure Object-Oriented Programming (OOP). The engine was intentionally designed to solve a specific constraint: maximizing real-time performance, stable frame rendering, and deterministic thread behavior on resource-constrained hardware profiles without relying on heavy third-party runtime frameworks like Unity or Unreal.

## ⚙️ Engine Sub-System Implementations & Technical Choices
- **Dynamic Object Pooling:** To prevent runtime heap fragmentation and garbage collection latency spikes during continuous object instantiation (such as procedural environment entities or projectiles), I engineered a custom memory management system. It pre-allocates entity records in a contiguous memory block and recycles inactive pointers, maintaining highly stable frame times.
- **Hand-Coded Spatial Partitioning & Collision Grids:** Rather than using external physics packages, I designed a low-latency bounding box collision grid using primitive multi-dimensional array indices. I hand-coded the underlying 3D vector transformations, dot/cross products, and projection matrices required to map coordinate spaces.
- **Decoupled Execution Architecture:** The engine isolates core systems onto separate, synchronized execution loops. The rendering update sequence, background physics calculation routines, and basic automated entity behavioral matrices run independently to ensure localized bottlenecks do not freeze the main simulation thread.

## 📊 Key Challenges & Technical Lessons
- **Memory Allocation Control:** The biggest hurdles were pointer safety and tracking down invisible memory leaks. Building this project cemented my understanding of manual memory allocation, low-level hardware constraints, and the vital role deterministic software execution plays when designing intensive real-time systems.

## 🧠 Relevance to Agentic AI Systems & Continuous Monitoring
Building simulation engines at this low level requires a deep mastery of linear algebra, runtime thread tracing, and handling system anomalies. This mathematical and programmatic foundation maps perfectly onto the technical requirements of developing complex distributed simulation loops and performance-oriented tracking tools for autonomous learning nodes.
