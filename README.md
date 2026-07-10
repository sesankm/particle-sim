# Data-Oriented Particle Simulation

A particle simulation using Verlet integration. Instead of tracking velocity during each frame, this method tracks current and previous positions to derive the next position of an object. 

This is an experiment in Data-Oriented Design to compare performance with traditional OOP, inspired by [Mike Acton's CppCon talk](https://www.youtube.com/watch?v=rX0ItVEVjHc$0).

https://github.com/user-attachments/assets/6c74e65a-7224-4aaa-8d0b-72d7c88ba91c

### Performance Comparisons:
- **OOP (Baseline)**: 1,854 particles
- **DoD (Single-threaded)**: 2,516 particles
- **DoD (Multi-threaded & optimized - WIP)**: 18,956 particles

Particle counts represent the max number of particles before the simulation dropped below 60fps. This isn't the most accurate benchmark, but it's enough to highlight how data layout alone can affect cache-hit rates.

The first two metrics give a clean, apples-to-apples comparison between DoD and OOP. Third shows how performance scales with multithreading and further optimization.

### Optimizations In Final Version:
- Avoid `sqrt`: Compare squared distances first to avoid expensive `sqrt` operations on non overlapping particles. Normalizing the vectors still requires a `sqrt`, but this eliminates it for non-collisions.
- Spatial Partitioning: Use a grid to limit collision checks to particles that are enclosed in the same or neighboring cells (O(N^2) vs O(N)).
- Persistent Threadpool: Keep threads alive and notify them when there's work, instead of creating/destroying threads each frame.
- Avoiding global mutex: Replaced single global mutex with localized cell-wise mutexes. This ensures thread safety while minimizing lock contention.