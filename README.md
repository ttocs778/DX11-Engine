# DX11 Game Engine Prototype

A small game engine written from scratch in C++ on top of DirectX 11. Each folder is one step of the build, and each step is a complete, runnable project that includes everything from the steps before it.

| Folder | What it adds |
|---|---|
| `01_Math` | Vector, matrix and quaternion library, plus an SSE (SIMD) version, with unit tests |
| `02_Triangle` | D3D11 device and swap chain, shader compiling, vertex buffers, first triangle |
| `03_Camera` | Camera, per-camera and per-object constant buffers, a 3D cube |
| `04_Texture` | Texture loading (DDS / WIC) and sampling |
| `05_Lighting` | Materials, Phong lighting with up to 8 point lights |
| `06_ModelsAndScene` | Mesh and material files, component system, JSON level loading, asset cache |
| `07_SkeletalAnimation` | Skeletons, keyframe interpolation, GPU skinning (4 bones per vertex) |
| `08_Collision` | AABB and line-segment tests, raycasts, player state machine (idle / walk / jump / fall / land) |
| `09_JobSystem` | 4-thread job system for animation, scoped profiler with chrome://tracing output |
| `10_NormalMapping` | Tangent-space normal mapping |
| `11_Bloom` | Off-screen render targets, bright-pass, downsampling, separable Gaussian blur, additive composite |
| `12_IK` | FABRIK inverse kinematics layered on top of the animation |
| `PoolAllocator` | A fixed-size pool allocator with a free list, with unit tests (standalone) |

## Building

Requires Windows and Visual Studio with the "Desktop development with C++" workload.

Open `<folder>/Game.sln` (or `01_Math/Lab01_Math/Lab01_Math.sln`) and build `x64`. DirectXTK is compiled from source in `external/` as part of the build. `11_Bloom` is the most complete version of the engine.

## Third-party

- [DirectXTK](https://github.com/microsoft/DirectXTK) for loading textures
- [RapidJSON](https://github.com/Tencent/rapidjson) for reading levels, meshes and animations
