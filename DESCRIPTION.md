# Project Description

## miniRT - Distributed Ray Tracing Renderer

miniRT is an advanced ray tracing renderer developed as part of the 42 School curriculum. It combines cutting-edge rendering techniques with distributed computing to create photorealistic images at interactive framerates.

### Overview

This project implements a fully-featured ray tracing engine in C that supports:
- **Physically-based rendering** with realistic light simulation
- **Distributed rendering** across multiple machines using a master-worker architecture
- **BVH acceleration structures** for 500x performance improvement on complex scenes
- **Multi-threaded rendering** utilizing all available CPU cores
- **Interactive camera control** with real-time scene navigation

### Key Features

#### Rendering Capabilities
- **Material System**: Lambertian (diffuse), Metal (reflective), and Dielectric (glass) materials
- **Advanced Effects**: Soft shadows, anti-aliasing, bump mapping, and checker textures
- **Global Illumination**: Recursive ray tracing with accurate light transport
- **Monte Carlo Sampling**: Adaptive anti-aliasing with 8-64 samples per pixel

#### Performance Optimization
- **BVH Acceleration**: Hierarchical spatial indexing reduces complexity from O(n) to O(log n)
- **Multi-threading**: Automatic CPU core detection and stripe-based work distribution
- **Compiler Optimizations**: Aggressive flags including `-Ofast`, LTO, and SIMD vectorization
- **Cache-optimized**: Memory layout designed for spatial and temporal locality

#### Distributed Computing
- **Master-Worker Architecture**: Coordinate rendering across multiple machines
- **Dynamic Load Balancing**: Tile-based work distribution ensures even load
- **Network Protocol**: Custom binary protocol for efficient data transmission
- **Fault Tolerance**: Workers can disconnect/reconnect without losing progress

#### Supported Primitives
- Sphere
- Plane
- Cylinder with caps
- Cone
- Pyramid
- Triangle (for mesh support)

### Technical Implementation

The renderer is built with:
- **~15,000 lines** of carefully optimized C code
- **80+ source files** organized into modular components
- **Custom math library** for vector and color operations
- **MLX42 graphics library** for cross-platform window management
- **POSIX threads** for concurrent rendering
- **Berkeley sockets** for network communication

### Scene Format

Scenes are defined using a simple text-based `.rt` format:

```rt
# Ambient lighting
A   0.15   255,255,255

# Camera (position orientation FOV)
C   0,1,5   0,0,0.5   70

# Light source
L   -5,8,3   0.8

# Objects with materials
sp  0,0,-1   1.2   255,255,255   M           # Metal sphere
sp  3,0,-1   1.2   50,205,50     L bm:earth.png:1.8   # Textured sphere
sp  0,3,-1.8 0.4   240,230,140   G:1.5       # Glass sphere
```

### Performance Benchmarks

**BVH Acceleration:**
- 10,000 spheres without BVH: 0.01 FPS (100s per frame)
- 10,000 spheres with BVH: 5.00 FPS (0.2s per frame)
- **500x speedup**

**Distributed Rendering:**
- 8 workers achieve 6.67x speedup with near-linear scaling
- Tile-based work distribution ensures optimal load balancing

### Educational Value

miniRT demonstrates proficiency in:
- **Computer Graphics**: Ray tracing algorithms, lighting models, material systems
- **Systems Programming**: Multi-threading, network protocols, memory management
- **Algorithm Design**: Spatial data structures, optimization techniques
- **Software Architecture**: Modular design, separation of concerns, clean interfaces

### Use Cases

- **Learning Tool**: Understand ray tracing fundamentals and advanced techniques
- **Render Farm**: Distribute heavy rendering across multiple machines
- **Prototyping**: Quick visualization of 3D scenes
- **Performance Study**: Benchmark different acceleration structures and algorithms

### Development Team

Developed by 4n4k1n and nweber23 as part of the 42 School curriculum, with extensive optimizations and distributed rendering capabilities beyond the base requirements.

### License

This project is open source under the MIT License.

---

For detailed technical documentation, build instructions, and usage examples, see the [README.md](README.md) and [CODE_DOCUMENTATION.md](docs/CODE_DOCUMENTATION.md).
