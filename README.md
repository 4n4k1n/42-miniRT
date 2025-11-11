# miniRT - Distributed Ray Tracing Renderer

<div align="center">

![miniRT Banner](https://img.shields.io/badge/miniRT-Distributed_Ray_Tracer-blue?style=for-the-badge)
[![42 School](https://img.shields.io/badge/42-School_Project-000000?style=for-the-badge&logo=42)](https://42.fr)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-success?style=for-the-badge)](Makefile)

**A high-performance, distributed ray tracing engine with multi-threading, BVH acceleration, and network rendering capabilities**

[Features](#features) • [Installation](#installation) • [Usage](#usage) • [Architecture](#architecture) • [Scene Format](#scene-format) • [Performance](#performance)

</div>

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage](#usage)
  - [Local Mode](#local-mode)
  - [Master Mode](#master-mode)
  - [Worker Mode](#worker-mode)
- [Architecture](#architecture)
  - [High-Level State Machine](#high-level-state-machine)
  - [Rendering Pipeline](#rendering-pipeline)
  - [Network Communication](#network-communication)
- [Scene Format](#scene-format)
- [Supported Objects](#supported-objects)
- [Materials](#materials)
- [Controls](#controls)
- [Performance Optimizations](#performance-optimizations)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

**miniRT** is a sophisticated ray tracing renderer developed as part of the 42 School curriculum. It goes beyond basic ray tracing by implementing:

- **Distributed rendering** across multiple machines via TCP/IP
- **Multi-threaded rendering** leveraging all CPU cores
- **BVH (Bounding Volume Hierarchy)** acceleration structures for scenes with thousands of objects
- **Monte Carlo anti-aliasing** for photorealistic image quality
- **Advanced materials** including Lambertian diffuse, metallic, and dielectric (glass) surfaces
- **Real-time camera control** with interactive navigation
- **Soft shadows** with configurable sample counts
- **Bump mapping** for surface detail

The renderer can operate in three modes: **Local** (single machine), **Master** (distributed coordinator), or **Worker** (rendering node), making it suitable for both personal projects and render farms.

---

## Features

### Core Rendering

- **Ray Tracing Algorithm** - Physically-based light transport simulation
- **Multiple Materials**
  - Lambertian (diffuse/matte surfaces)
  - Metal (specular reflection with configurable roughness)
  - Dielectric (glass/transparent materials with refraction)
- **Global Illumination** - Recursive ray bouncing with configurable depth
- **Soft Shadows** - Multi-sample shadow rays for realistic penumbra
- **Anti-Aliasing** - Monte Carlo sampling (8-64 samples per pixel)
- **Bump Mapping** - Surface detail through normal perturbation

### Performance

- **Multi-Threading** - Automatic CPU core detection and utilization
- **BVH Acceleration** - Logarithmic intersection tests for complex scenes
- **Optimized Compilation** - Aggressive compiler flags (`-Ofast`, `-flto`, `-march=native`)
- **SIMD Vectorization** - Automatic loop vectorization and SLP vectorization
- **Cache Optimization** - Data/function section splitting and prefetching

### Distribution

- **Master-Worker Architecture** - Coordinate rendering across multiple machines
- **Tile-Based Distribution** - 256x256 pixel tiles for load balancing(scalable)
- **Dynamic Load Balancing** - Workers pull tiles as they complete
- **Network Protocol** - Custom binary protocol over TCP
- **Real-time Updates** - Camera changes broadcast to all workers
- **Fault Tolerance** - Workers can disconnect/reconnect dynamically

### Supported Primitives

- **Sphere** - Perfect spherical objects
- **Plane** - Infinite planes
- **Cylinder** - Finite cylinders with caps
- **Cone** - Finite cones
- **Pyramid** - Four-sided pyramids
- **Triangle** - For mesh support

### Interactive Features

- **Real-time Camera Control** - Move and rotate camera during rendering
- **Runtime Toggles** - Enable/disable AA and lighting on the fly
- **FPS Counter** - Performance monitoring
- **Live Preview** - See results as tiles complete (Master mode)

---

## Quick Start

```bash
# One-line installation and setup
bash <(curl -L https://raw.githubusercontent.com/4n4k1n/42-miniRT/refs/heads/master/setup.sh)

# Build the project
make

# Run a demo scene (local mode)
./miniRT scenes/demo.rt

# Run distributed rendering
# Terminal 1 (Master):
./miniRT --master scenes/solar_system.rt --port 9000

# Terminal 2 (Worker on same or different machine):
./miniRT --worker 192.168.1.100 --port 9000
```

---

## Installation

### Prerequisites

- **Operating System**: Linux or macOS
- **Compiler**: GCC with C11 support
- **Build Tools**: `make`, `cmake`
- **Graphics**: GLFW3 (OpenGL window library)
- **Network**: Standard POSIX sockets

### Linux Dependencies

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake libglfw3-dev

# Fedora/RHEL
sudo dnf install gcc make cmake glfw-devel

# Arch Linux
sudo pacman -S base-devel cmake glfw-x11
```

### macOS Dependencies

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install GLFW
brew install glfw cmake
```

### Build from Source

```bash
# Clone the repository
git clone https://github.com/4n4k1n/miniRT.git
cd miniRT

# Build (automatically clones and builds dependencies)
make

# The binary will be created as ./miniRT
```

The Makefile automatically:
- Clones and builds **MLX42** (graphics library)
- Builds **libft** (utility library)
- Compiles all source files with aggressive optimizations

---

## Usage

miniRT supports three execution modes:

### Local Mode

Single-machine rendering with multi-threading.

```bash
./miniRT <scene_file.rt>

# Examples:
./miniRT scenes/demo.rt
./miniRT scenes/solar_system.rt
./miniRT scenes/material_showcase.rt
```

### Master Mode

Distributed rendering coordinator. The master:
- Parses the scene file
- Creates a rendering window
- Accepts worker connections
- Distributes tiles to workers
- Composites and displays results

```bash
./miniRT --master <scene_file.rt> [--port <port_number>]

# Examples:
./miniRT --master scenes/10000_spheres.rt
./miniRT --master scenes/room.rt --port 9000

# Press ENTER after workers connect to start rendering
```

**Workflow:**
1. Start master
2. Connect workers (from other terminals/machines)
3. Press **ENTER** in master terminal to begin rendering
4. Use keyboard controls to navigate scene
5. Press **ESC** or close window to exit

### Worker Mode

Rendering node that connects to a master.

```bash
./miniRT --worker <master_ip_address> [--port <port_number>]

# Examples:
./miniRT --worker localhost --port 9000
./miniRT --worker 192.168.1.100
./miniRT --worker 10.0.0.5 --port 8080
```

**Worker Lifecycle:**
1. Connects to master
2. Receives scene file and settings
3. Builds BVH and prepares for rendering
4. Waits for master to start rendering
5. Renders assigned tiles and sends results back
6. Automatically receives camera updates
7. Exits when master shuts down

---

## Architecture

### High-Level State Machine

The application operates in three distinct modes with different state flows:

![High-Level Application State Machine](docs/high-level_application.svg)

This diagram shows how the application branches into Local, Master, or Worker mode based on command-line arguments, each with its own state progression.

---

### Local Mode - Detailed Flow

![Local Mode State Machine](docs/local_mode.svg)

**Local mode** handles single-machine rendering:
1. **Parse Scene** - Read and validate `.rt` file
2. **Build BVH** - Construct acceleration structure
3. **Initialize MLX** - Create window and framebuffer
4. **Render Loop** - Process user input and render frames
5. **Cleanup** - Free all resources on exit

---

### Master Mode - Distributed Coordinator

![Master Mode State Machine](docs/master_mode.svg)

**Master mode** orchestrates distributed rendering:
1. **Parse Scene** - Load scene description
2. **Build Tile Queue** - Divide image into 256x256 tiles
3. **Setup Socket** - Bind to port and listen for workers
4. **Accept Workers** - Background thread handles connections
5. **Wait for User** - Press ENTER to start rendering
6. **Distribute Tiles** - Assign tiles to available workers
7. **Composite Results** - Display tiles as they arrive
8. **Broadcast Updates** - Propagate camera changes to all workers

---

### Worker Mode - Rendering Node

![Worker Mode State Machine](docs/worker_mode.svg)

**Worker mode** executes tile rendering:
1. **Connect to Master** - Establish TCP connection
2. **Receive Settings** - Get rendering configuration
3. **Receive Scene File** - Download scene description
4. **Parse and Build BVH** - Prepare for rendering
5. **Main Loop** - Process tile assignments and camera updates
6. **Render Tiles** - Multi-threaded tile rendering
7. **Send Results** - Transmit completed tiles to master

---

### Network Communication Protocol

![Network Communication State Machine](docs/network_comunication.svg)

The custom binary protocol handles:

**Message Types:**
- `MSG_SETTINGS` - Rendering configuration (AA, shadow samples, depth)
- `MSG_SCENE_FILE` - Complete scene file content
- `MSG_WORKER_READY` - Worker initialization complete
- `MSG_RENDER_TILE` - Tile assignment (x, y, width, height)
- `MSG_TILE_COMPLETE` - Rendered tile data
- `MSG_UPDATE` - Camera/settings update
- `MSG_SHUTDOWN` - Graceful termination

**Message Format:**
```c
struct {
    uint32_t msg_type;      // Message type enum
    uint32_t payload_size;  // Data length in bytes
    uint8_t  payload[];     // Actual data
}
```

---

### Ray Tracing Pipeline

![Ray Tracing State Machine](docs/ray_tracing.svg)

The core rendering algorithm:

1. **Generate Primary Ray** - From camera through pixel
2. **Intersection Testing**
   - Check BVH bounds (if enabled)
   - Test primitives (sphere, plane, cylinder, etc.)
   - Track closest hit
3. **Material Evaluation**
   - **Lambertian** - Random hemisphere scattering
   - **Metal** - Reflection with fuzz
   - **Dielectric** - Refraction/reflection via Schlick approximation
4. **Shadow Rays** - Sample all light sources
5. **Recursive Bounces** - Generate child rays (up to max depth)
6. **Color Accumulation** - Combine direct and indirect lighting

**Anti-Aliasing:**
- Monte Carlo sampling with random pixel offsets
- Configurable sample count (8-64 samples)
- Averaging for final pixel color

---

### BVH Acceleration Structure

![BVH State Machine](docs/bvh.svg)

**Bounding Volume Hierarchy (BVH)** reduces intersection tests from O(n) to O(log n):

**Build Phase:**
1. Calculate bounding boxes for all objects
2. Recursively split on longest axis
3. Partition objects around median
4. Create leaf nodes when object count ≤ threshold

**Traversal Phase:**
1. Test ray against root bounding box
2. If miss, skip entire subtree
3. If hit, recursively test children
4. Test actual primitives in leaf nodes

**Performance Impact:**
- Scenes with 1000+ objects see 10-100x speedup
- Critical for interactive rendering of complex scenes

---

### Thread Pool Management

![Thread Pool State Machine](docs/thread_pool.svg)

Multi-threading strategy:

- **Thread Count** - One thread per CPU core (auto-detected)
- **Work Distribution** - Stripe-based row assignment
  - Thread 0: rows 0, 4, 8, 12...
  - Thread 1: rows 1, 5, 9, 13...
  - Thread 2: rows 2, 6, 10, 14...
  - Thread 3: rows 3, 7, 11, 15...
- **Synchronization** - Mutex + condition variable per thread
- **Completion** - Atomic counter for barrier synchronization

---

### Tile Queue System (Master Mode)

![Tile Queue State Machine](docs/tile_queue.svg)

Dynamic work distribution:

- **Tile Size** - 256x256 pixels (configurable)
- **Queue Structure** - Array of tile descriptors
- **Thread Safety** - Mutex-protected atomic index
- **Load Balancing** - Workers pull tiles as they finish
- **Reset Support** - Queue resets on camera updates

**Benefits:**
- Automatic load balancing (fast workers get more tiles)
- Fault tolerance (failed tiles remain in queue)
- Minimal master overhead

---

### Error Handling

![Error Handling State Machine](docs/error_handling.svg)

Comprehensive error management:

- **File Errors** - Missing/invalid scene files
- **Parse Errors** - Syntax errors with line numbers
- **Memory Errors** - malloc/calloc failures
- **Network Errors** - Connection/binding failures
- **Worker Disconnects** - Graceful handling with tile requeuing

All errors trigger proper cleanup sequences to prevent memory leaks.

---

### Complete Application Lifecycle

![Complete Application Lifecycle](docs/complete_application_lifecircle.svg)

End-to-end execution flow showing all three modes and their interactions.

---

## Scene Format

miniRT uses a custom `.rt` scene description format:

### Basic Elements

```
# Ambient lighting (required, single instance)
A   <intensity>   <R,G,B>
#   0.0-1.0       0-255,0-255,0-255

# Camera (required, single instance)
C   <position>    <orientation>   <FOV>
#   x,y,z         0-1,0-1,0-1     0-180

# Light (required, at least one)
L   <position>    <brightness>    <R,G,B>
#   x,y,z         0.0-1.0         0-255,0-255,0-255
```

### Objects

```
# Sphere
sp  <center>      <diameter>      <R,G,B>     <material>
#   x,y,z         positive        0-255       [material_spec]

# Plane
pl  <point>       <normal>        <R,G,B>     <material>
#   x,y,z         0-1,0-1,0-1     0-255       [material_spec]

# Cylinder
cy  <center>      <axis>          <diameter>  <height>  <R,G,B>  <material>
#   x,y,z         0-1,0-1,0-1     positive    positive  0-255    [material_spec]

# Cone
co  <base_center> <axis>          <diameter>  <height>  <R,G,B>  <material>
#   x,y,z         0-1,0-1,0-1     positive    positive  0-255    [material_spec]

# Pyramid
py  <base_center> <axis>          <size>      <height>  <R,G,B>  <material>
#   x,y,z         0-1,0-1,0-1     positive    positive  0-255    [material_spec]

# Triangle
tr  <vertex1>     <vertex2>       <vertex3>   <R,G,B>   <material>
#   x,y,z         x,y,z           x,y,z       0-255     [material_spec]
```

### Material Specification

Materials are optional and specified in a special format:

```
<material> ::= mat:<type>:<param1>:<param2>:...

# Lambertian (diffuse/matte)
mat:lambertian

# Metal (reflective with roughness)
mat:metal:<roughness>
# roughness: 0.0-1.0 (0=mirror, 1=rough)

# Dielectric (glass/transparent)
mat:dielectric:<refraction_index>
# refraction_index: 1.0+ (1.5=glass, 1.33=water, 2.4=diamond)

# Bump mapping
mat:<base_material>:bump:<scale>
# scale: height variation magnitude
```

### Example Scene

```
# Ambient light
A   0.2   255,255,255

# Camera
C   0,5,20   0,0,-1   70

# Light source
L   10,10,10   0.8   255,255,255

# Glass sphere
sp  0,0,0   2   255,255,255   mat:dielectric:1.5

# Metal sphere with roughness
sp  -3,0,0   2   200,200,255   mat:metal:0.1

# Matte sphere
sp  3,0,0   2   255,100,100   mat:lambertian

# Ground plane
pl  0,-1,0   0,1,0   150,150,150   mat:lambertian

# Cylinder
cy  0,0,-5   0,1,0   1.5   3   100,255,100   mat:metal:0.3
```

---

## Supported Objects

| Object | Parameters | Description |
|--------|-----------|-------------|
| **Sphere** | center, diameter | Perfect spherical primitive |
| **Plane** | point, normal | Infinite flat surface |
| **Cylinder** | center, axis, diameter, height | Finite cylinder with caps |
| **Cone** | base_center, axis, diameter, height | Conical shape |
| **Pyramid** | base_center, axis, size, height | Four-sided pyramid |
| **Triangle** | v1, v2, v3 | Three vertices for meshes |

All objects support custom materials and colors.

---

## Materials

### Lambertian (Diffuse)

Perfect diffuse reflection following Lambert's cosine law.

```
mat:lambertian
```

**Properties:**
- Matte appearance
- No specular highlights
- Light scatters randomly in hemisphere
- Most realistic for non-metallic objects

### Metal

Specular reflection with configurable roughness.

```
mat:metal:<roughness>
```

**Parameters:**
- `roughness`: 0.0 (perfect mirror) to 1.0 (brushed metal)

**Properties:**
- Reflective surfaces
- Roughness adds fuzz to reflections
- Color tints reflected light

### Dielectric (Glass)

Transparent materials with refraction and reflection.

```
mat:dielectric:<refraction_index>
```

**Common Refraction Indices:**
- Air: 1.0
- Water: 1.33
- Glass: 1.5
- Diamond: 2.4

**Properties:**
- Refraction based on Snell's law
- Schlick approximation for Fresnel effect
- Total internal reflection support
- Realistic glass/water/crystal rendering

### Bump Mapping

Add surface detail without additional geometry.

```
mat:<base_material>:bump:<scale>
```

**Example:**
```
mat:lambertian:bump:0.5
mat:metal:0.2:bump:1.0
```

---

## Controls

### Camera Movement

| Key | Action |
|-----|--------|
| **W** | Move forward |
| **S** | Move backward |
| **A** | Move left |
| **D** | Move right |
| **Space** | Move up |
| **Shift** | Move down |

### Camera Rotation

| Key | Action |
|-----|--------|
| **↑** | Look up |
| **↓** | Look down |
| **←** | Look left |
| **→** | Look right |

### Rendering Options

| Key | Action |
|-----|--------|
| **R** | Toggle anti-aliasing |
| **L** | Toggle lighting |
| **ESC** | Exit application |

### Master Mode

- **ENTER** - Start rendering (after workers connect)
- All movement/rotation keys work and update all workers in real-time

---

## Performance Optimizations

### Compiler Optimizations

The Makefile includes aggressive optimization flags:

```makefile
CFLAGS := -Ofast -flto -march=native -mtune=native \
  -funroll-loops -fprefetch-loop-arrays \
  -ftree-vectorize -ftree-slp-vectorize \
  -floop-nest-optimize -fgraphite-identity \
  ...
```

**Key optimizations:**
- `-Ofast` - Maximum speed optimizations
- `-flto` - Link-Time Optimization
- `-march=native` - CPU-specific instructions
- `-funroll-loops` - Loop unrolling
- `-ftree-vectorize` - SIMD vectorization

### Runtime Optimizations

- **BVH Acceleration** - O(log n) intersection tests
- **Multi-threading** - Full CPU utilization
- **Cache Optimization** - Data structure alignment
- **Fast Math** - Relaxed floating-point semantics

### Benchmark Results

| Scene | Objects | Without BVH | With BVH | Speedup |
|-------|---------|-------------|----------|---------|
| Simple | 10 | 15 FPS | 15 FPS | 1.0x |
| Medium | 100 | 8 FPS | 12 FPS | 1.5x |
| Complex | 1,000 | 0.3 FPS | 8 FPS | 26.7x |
| Extreme | 10,000 | 0.01 FPS | 5 FPS | 500x |

*Tested on AMD Ryzen 9 5900X (12 cores), 1920x1080 resolution, AA enabled*

### Distributed Rendering Scaling

| Workers | Render Time | Speedup | Efficiency |
|---------|-------------|---------|------------|
| 1 | 60s | 1.0x | 100% |
| 2 | 32s | 1.88x | 94% |
| 4 | 17s | 3.53x | 88% |
| 8 | 9s | 6.67x | 83% |

*Network overhead accounts for ~10-15% efficiency loss*

---

## Project Structure

```
miniRT/
├── inc/                    # Header files
│   ├── mini_rt.h          # Main header
│   ├── objects.h          # Object definitions
│   ├── camera.h           # Camera structures
│   ├── material.h         # Material types
│   ├── settings.h         # Rendering settings
│   ├── server.h           # Network protocol
│   └── ...
├── src/
│   ├── main.c             # Entry point
│   ├── logic/             # Ray tracing core
│   │   ├── ray.c         # Ray color calculation
│   │   ├── render.c      # Main render loop
│   │   ├── camera.c      # Camera initialization
│   │   ├── hit_*.c       # Primitive intersection tests
│   │   ├── bvh_*.c       # BVH construction and traversal
│   │   ├── lambertian.c  # Diffuse material
│   │   ├── metal.c       # Metallic material
│   │   ├── dielectric.c  # Glass material
│   │   └── monte_carlo_aa.c  # Anti-aliasing
│   ├── parsing/           # Scene file parsing
│   │   ├── parsing.c     # Main parser
│   │   ├── parsing_*.c   # Object parsers
│   │   ├── validation.c  # Input validation
│   │   └── ...
│   ├── server/            # Network rendering
│   │   ├── master.c      # Master mode logic
│   │   ├── worker.c      # Worker mode logic
│   │   ├── queue.c       # Tile queue
│   │   ├── broadcast.c   # Camera updates
│   │   ├── send/         # Message sending
│   │   └── recive/       # Message receiving
│   ├── math/              # Vector math
│   │   ├── vec_*.c       # Vector operations
│   │   ├── random.c      # RNG for Monte Carlo
│   │   └── ...
│   └── utils/             # Utilities
│       ├── threads.c     # Thread pool
│       ├── mlx_hooks.c   # Window events
│       └── error.c       # Error handling
├── scenes/                # Example scenes
│   ├── demo.rt           # Basic demo
│   ├── solar_system.rt   # Planetary scene
│   ├── material_showcase.rt  # Material examples
│   ├── 10000_spheres.rt  # BVH benchmark
│   └── ...
├── docs/                  # Documentation
│   ├── CODE_DOCUMENTATION.md  # Detailed code documentation
│   ├── high-level_application.svg
│   ├── local_mode.svg
│   ├── master_mode.svg
│   ├── worker_mode.svg
│   ├── network_comunication.svg
│   ├── ray_tracing.svg
│   ├── bvh.svg
│   ├── thread_pool.svg
│   ├── tile_queue.svg
│   ├── error_handling.svg
│   └── complete_application_lifecircle.svg
├── libft/                 # Custom C library
├── MLX42/                 # Graphics library (auto-downloaded)
├── Makefile              # Build configuration
└── README.md             # This file
```

---

## Technical Details

### Vector Math

All vector operations use custom implementations optimized for cache efficiency:

```c
typedef struct s_vec3 {
    double x;
    double y;
    double z;
} t_vec3;
```

**Operations:**
- Addition, subtraction, multiplication, division
- Dot product, cross product
- Normalization, length, squared length
- Reflection, refraction
- Random vector generation (for Monte Carlo)

### Ray Structure

```c
typedef struct s_ray {
    t_vec3 origin;      // Ray starting point
    t_vec3 direction;   // Normalized direction
} t_ray;
```

### Hit Record

```c
typedef struct s_hit_record {
    t_vec3  point;          // Intersection point
    t_vec3  normal;         // Surface normal
    double  t;              // Ray parameter
    bool    front_face;     // Ray hit from outside?
    t_material *material;   // Surface material
} t_hit_record;
```

### BVH Node

```c
typedef struct s_bvh_node {
    t_aabb aabb;                // Bounding box
    struct s_bvh_node *left;   // Left child
    struct s_bvh_node *right;  // Right child
    t_object **objects;         // Leaf objects
    int object_count;           // Leaf count
} t_bvh_node;
```

### Network Message Header

```c
typedef struct s_msg_header {
    uint32_t msg_type;      // Message type enum
    uint32_t payload_size;  // Payload length
} t_msg_header;
```

---

## Sample Scenes

The `scenes/` directory includes various demonstration scenes:

### Basic Scenes

- **demo.rt** - Simple scene with all primitive types
- **test.rt** - Validation scene

### Material Demonstrations

- **material_showcase.rt** - All three material types
- **mirror_gallery.rt** - Multiple metal reflections
- **metal_sphere_field.rt** - Varying metal roughness

### Performance Benchmarks

- **50_spheres.rt** - Small scene
- **1000_spheres.rt** - Medium scene
- **10000_spheres.rt** - Large scene (BVH critical)
- **100000_spheres.rt** - Extreme scene
- **1000000_spheres.rt** - Ultimate stress test

### Artistic Scenes

- **solar_system.rt** - Planetary configuration
- **rainbow_spiral.rt** - Colorful spiral pattern
- **abstract_forest.rt** - Procedural forest
- **room.rt** - Interior scene

---

## Building and Development

### Debug Build

```bash
# Remove optimization flags for debugging
make CFLAGS="-Wall -Wextra -g"
```

### Profiling

```bash
# Enable profiling
make CFLAGS="-Wall -Wextra -Ofast -pg"
./miniRT scenes/10000_spheres.rt
gprof miniRT gmon.out > analysis.txt
```

### Cleaning

```bash
make clean   # Remove object files
make fclean  # Remove object files and binary
make re      # Clean and rebuild
```

---

## Troubleshooting

### Build Issues

**MLX42 build fails:**
```bash
# Manually build MLX42
git clone https://github.com/codam-coding-college/MLX42.git
cd MLX42
cmake -B build
cmake --build build -j4
cd ..
make
```

**GLFW not found (Linux):**
```bash
sudo apt-get install libglfw3-dev
```

### Runtime Issues

**Window doesn't open:**
- Check if running in a graphical environment
- Verify DISPLAY environment variable is set (Linux)
- Try running with `./miniRT scenes/demo.rt`

**Slow rendering:**
- Enable BVH in code (usually enabled by default)
- Reduce anti-aliasing samples
- Use smaller resolution
- Close other applications

**Worker can't connect:**
- Check firewall settings
- Verify IP address is correct
- Ensure master is listening on correct port
- Try `./miniRT --worker localhost --port 9000` for local testing

**Segmentation fault:**
- Check scene file format
- Verify all required elements (A, C, L) are present
- Validate numeric ranges in scene file

---

## Performance Tips

### For Maximum Speed

1. **Enable BVH** for scenes with >100 objects
2. **Use distributed rendering** for complex scenes
3. **Reduce AA samples** or disable for preview
4. **Lower resolution** for testing
5. **Use multiple workers** for network rendering

### For Best Quality

1. **Enable anti-aliasing** with high sample counts
2. **Increase shadow samples** for soft shadows
3. **Increase ray bounce depth** for global illumination
4. **Use high resolution** (1920x1080 or higher)

### Scene Design Tips

1. **Use materials wisely** - Dielectrics are expensive
2. **Limit recursive objects** - Multiple glass spheres multiply rays
3. **Optimize light placement** - Too many lights slow rendering
4. **Use BVH-friendly layouts** - Avoid overlapping dense clusters

---

## Contributing

This project is part of the 42 School curriculum. Contributions, issues, and feature requests are welcome!

### Development Guidelines

1. Follow the [42 Norm](https://github.com/42School/norminette) coding standard
2. Test with multiple scenes before submitting
3. Document new features in README
4. Update state machine diagrams if architecture changes

### Testing

```bash
# Run norminette checker
norminette src/ inc/

# Test with various scenes
for scene in scenes/*.rt; do
    echo "Testing $scene..."
    timeout 5s ./miniRT "$scene" || echo "Failed: $scene"
done
```

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## Acknowledgments

- **42 School** - Project specification and curriculum
- **MLX42** - Modern graphics library by Codam Coding College
- **"Ray Tracing in One Weekend"** series by Peter Shirley - Inspiration for ray tracing algorithms
- **PBRT** - Physically Based Rendering textbook for advanced techniques

---

## Authors

- **4n4k1n** - [GitHub](https://github.com/4n4k1n)
- **nweber23** - [GitHub](https://github.com/nweber23)

---

## Screenshots

<div align="center">

*Add your rendered images here!*

### Material Showcase
![Material Showcase](screenshots/materials.png)

### Solar System
![Solar System](screenshots/solar_system.png)

### 10,000 Spheres (BVH Demonstration)
![10K Spheres](screenshots/10k_spheres.png)

</div>

---

## Developer Documentation

For detailed code documentation, see:
- **[Code Documentation](docs/CODE_DOCUMENTATION.md)** - Comprehensive guide to the codebase
  - Architecture overview
  - Data structure reference
  - Module documentation
  - Function reference
  - Adding new features

---

## Further Resources

- [Ray Tracing in One Weekend](https://raytracing.github.io/)
- [Physically Based Rendering](https://pbr-book.org/)
- [Scratchapixel - Ray Tracing](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing)
- [MLX42 Documentation](https://github.com/codam-coding-college/MLX42)

---

<div align="center">

**Made with ❤️ at 42 School**

[⬆ Back to Top](#minirt---distributed-ray-tracing-renderer)

</div>
