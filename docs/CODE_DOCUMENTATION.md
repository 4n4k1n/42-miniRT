# miniRT Code Documentation

This document provides detailed documentation of the miniRT codebase, including module descriptions, function references, data structures, and implementation details.

---

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Core Data Structures](#core-data-structures)
3. [Module Documentation](#module-documentation)
   - [Main Entry Point](#main-entry-point)
   - [Ray Tracing Core](#ray-tracing-core)
   - [Scene Parsing](#scene-parsing)
   - [Network Rendering](#network-rendering)
   - [Math Library](#math-library)
   - [Utilities](#utilities)
4. [Function Reference](#function-reference)
5. [Compilation and Build System](#compilation-and-build-system)
6. [Adding New Features](#adding-new-features)

---

## Architecture Overview

miniRT follows a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────┐
│                    Main Entry Point                      │
│                      (src/main.c)                        │
└────────────┬────────────────────────────────────────────┘
             │
      ┌──────┴──────┐
      │             │
      ▼             ▼
┌───────────┐  ┌──────────────┐
│  Local    │  │ Distributed  │
│  Renderer │  │ Renderer     │
└─────┬─────┘  └──────┬───────┘
      │               │
      │     ┌─────────┴─────────┐
      │     │                   │
      │     ▼                   ▼
      │  ┌────────┐      ┌────────┐
      │  │ Master │      │ Worker │
      │  └────┬───┘      └───┬────┘
      │       │              │
      └───────┴──────┬───────┘
                     │
          ┌──────────┴──────────┐
          │                     │
          ▼                     ▼
    ┌──────────┐         ┌──────────┐
    │ Parsing  │         │ Rendering│
    │ Module   │         │ Engine   │
    └────┬─────┘         └────┬─────┘
         │                    │
         ▼                    ▼
    ┌─────────┐         ┌──────────┐
    │ Scene   │         │ Ray      │
    │ Objects │◄────────┤ Tracing  │
    └─────────┘         └────┬─────┘
                             │
                   ┌─────────┴─────────┐
                   │                   │
                   ▼                   ▼
            ┌────────────┐      ┌──────────┐
            │ Materials  │      │   BVH    │
            └────────────┘      └──────────┘
```

---

## Core Data Structures

### 1. Vector (t_vec3)

**Location:** `inc/own_math.h`

```c
typedef struct s_vec3 {
    double x;
    double y;
    double z;
} t_vec3;
```

**Purpose:** Represents 3D vectors for positions, directions, normals, and colors.

**Key Operations:**
- Arithmetic: `vec_add()`, `vec_sub()`, `vec_multiply()`, `vec_divide()`
- Products: `vec_dot()`, `vec_cross()`
- Normalization: `vec_normalize()`, `vec_sqrt()`
- Utilities: `vec_zero()`, `vec_cpy()`

---

### 2. RGB Color (t_rgb)

**Location:** `inc/own_math.h`

```c
typedef struct s_rgb {
    double r;  // Red (0.0 - 1.0)
    double g;  // Green (0.0 - 1.0)
    double b;  // Blue (0.0 - 1.0)
} t_rgb;
```

**Purpose:** Color representation in floating-point format for accurate calculations.

**Key Operations:**
- `rgb_add()` - Add two colors
- `rgb_modulate()` - Multiply colors component-wise
- `rgb_multiply()` - Scale color by scalar
- `rgb_to_uint32()` - Convert to 32-bit display format

---

### 3. Ray (t_ray)

**Location:** `inc/hittable.h`

```c
typedef struct s_ray {
    t_vec3 origin;      // Starting point
    t_vec3 direction;   // Normalized direction vector
} t_ray;
```

**Purpose:** Represents a ray cast into the scene.

**Usage:**
```c
t_ray ray;
ray.origin = camera_position;
ray.direction = normalize(pixel_direction);
t_rgb color = ray_color(&ray, data, 0);
```

---

### 4. Material (t_material)

**Location:** `inc/material.h`

```c
typedef enum e_material_type {
    LAMBERTIAN,   // Diffuse reflection
    METAL,        // Specular reflection
    DIELECTRIC    // Refraction/reflection
} t_material_type;

typedef struct s_material {
    t_material_type type;
    t_rgb           albedo;       // Base color
    double          param;        // Roughness (metal) or IOR (dielectric)
    t_vec3          (*bump)(t_vec3 *point);  // Bump function pointer
} t_material;
```

**Purpose:** Defines surface properties for realistic rendering.

**Material Types:**

1. **LAMBERTIAN** - Perfect diffuse reflection
   - `param` unused
   - Scatters rays randomly in hemisphere

2. **METAL** - Specular reflection
   - `param` = roughness (0.0 = mirror, 1.0 = brushed)
   - Reflects rays with optional fuzz

3. **DIELECTRIC** - Glass/transparent materials
   - `param` = index of refraction (1.5 for glass)
   - Handles refraction and reflection (Schlick approximation)

---

### 5. Hit Record (t_hit_record)

**Location:** `inc/hittable.h`

```c
typedef struct s_hit_record {
    t_vec3      point;       // Intersection point
    t_vec3      normal;      // Surface normal at hit point
    double      t;           // Ray parameter (distance)
    bool        front_face;  // True if ray hit from outside
    t_material  *material;   // Material at intersection
} t_hit_record;
```

**Purpose:** Stores information about ray-object intersection.

**Usage:**
```c
t_hit_record rec;
if (hit_sphere(sphere, ray, &rec)) {
    // rec.point contains intersection point
    // rec.normal contains surface normal
    // rec.material contains surface properties
}
```

---

### 6. Object (t_object)

**Location:** `inc/objects.h`

```c
typedef enum e_object_type {
    SPHERE,
    PLANE,
    CYLINDER,
    CONE,
    PYRAMID,
    TRIANGLE
} t_object_type;

typedef struct s_object {
    t_object_type   type;
    void            *data;        // Type-specific data (t_sphere*, t_plane*, etc.)
    t_material      material;
    struct s_object *next;        // Linked list
} t_object;
```

**Purpose:** Generic container for all renderable objects.

**Type-Specific Data Structures:**

```c
// Sphere
typedef struct s_sphere {
    t_vec3  center;
    double  radius;
    t_rgb   color;
} t_sphere;

// Plane
typedef struct s_plane {
    t_vec3  point;
    t_vec3  normal;
    t_rgb   color;
} t_plane;

// Cylinder
typedef struct s_cylinder {
    t_vec3  center;
    t_vec3  axis;
    double  radius;
    double  height;
    t_rgb   color;
} t_cylinder;

// Cone
typedef struct s_cone {
    t_vec3  base_center;
    t_vec3  axis;
    double  radius;
    double  height;
    t_rgb   color;
} t_cone;

// Pyramid
typedef struct s_pyramid {
    t_vec3  base_center;
    t_vec3  axis;
    double  size;
    double  height;
    t_rgb   color;
} t_pyramid;

// Triangle
typedef struct s_triangle {
    t_vec3  v0, v1, v2;  // Vertices
    t_vec3  normal;      // Precomputed normal
    t_rgb   color;
} t_triangle;
```

---

### 7. Camera (t_camera)

**Location:** `inc/camera.h`

```c
typedef struct s_camera {
    t_vec3  position;           // Camera world position
    t_vec3  orientation;        // Normalized look-at direction
    double  fov;                // Field of view (degrees)

    // Computed values (filled by init_camera)
    t_vec3  lower_left_corner;  // Viewport bottom-left
    t_vec3  horizontal;         // Viewport horizontal span
    t_vec3  vertical;           // Viewport vertical span
    double  viewport_height;
    double  viewport_width;
} t_camera;
```

**Purpose:** Defines viewpoint and generates primary rays.

**Initialization:**
```c
void init_camera(t_data *data) {
    double aspect_ratio = (double)data->width / data->height;
    double theta = data->camera.fov * M_PI / 180.0;
    double h = tan(theta / 2.0);
    data->camera.viewport_height = 2.0 * h;
    data->camera.viewport_width = aspect_ratio * data->camera.viewport_height;

    // Compute basis vectors and viewport corners
    // ...
}
```

---

### 8. Light (t_light)

**Location:** `inc/objects.h`

```c
typedef struct s_light {
    t_vec3          position;
    double          brightness;  // 0.0 - 1.0
    t_rgb           color;
    struct s_light  *next;       // Linked list
} t_light;
```

**Purpose:** Point light source for illumination.

**Shadow Ray Calculation:**
```c
for (t_light *light = data->lights; light; light = light->next) {
    t_vec3 light_dir = vec_sub(&light->position, &rec.point);
    // Cast shadow ray
    // Accumulate lighting contribution
}
```

---

### 9. BVH Node (t_bvh_node)

**Location:** `inc/hittable.h`

```c
typedef struct s_aabb {
    t_vec3 min;  // Minimum corner
    t_vec3 max;  // Maximum corner
} t_aabb;

typedef struct s_bvh_node {
    t_aabb              aabb;         // Bounding box
    struct s_bvh_node   *left;        // Left child
    struct s_bvh_node   *right;       // Right child
    t_object            **objects;    // Leaf node objects
    int                 object_count; // Number of objects in leaf
} t_bvh_node;
```

**Purpose:** Acceleration structure for fast ray-object intersection.

**Tree Structure:**
- **Interior nodes**: Have `left` and `right` children, no objects
- **Leaf nodes**: Have `objects` array, null children

**Traversal:**
```c
bool bvh_hit(t_bvh_node *node, t_ray *ray, t_hit_record *rec) {
    if (!aabb_hit(&node->aabb, ray))
        return false;  // Early exit - ray misses bounding box

    if (node->left == NULL) {
        // Leaf node - test all objects
        return hit_objects(node->objects, node->object_count, ray, rec);
    }

    // Interior node - test children
    bool hit_left = bvh_hit(node->left, ray, rec);
    bool hit_right = bvh_hit(node->right, ray, rec);
    return hit_left || hit_right;
}
```

---

### 10. Rendering Settings (t_settings)

**Location:** `inc/settings.h`

```c
typedef struct s_settings {
    bool    aa_state;        // Anti-aliasing enabled
    bool    light_state;     // Lighting enabled
    int     min_samples;     // Min AA samples per pixel
    int     max_samples;     // Max AA samples per pixel
    int     shadow_samples;  // Shadow ray samples per light
    int     depth;           // Max ray bounce depth
    bool    use_bvh;         // BVH acceleration enabled
} t_settings;
```

**Default Values:**
```c
settings.aa_state = true;
settings.min_samples = 8;
settings.max_samples = 64;
settings.shadow_samples = 4;
settings.depth = 50;
settings.use_bvh = true;
```

---

### 11. Thread Pool (t_thread)

**Location:** `inc/mini_rt.h`

```c
typedef struct s_thread {
    pthread_t           thread;       // Thread handle
    pthread_mutex_t     active_mutex; // Protects active state
    pthread_cond_t      active_cond;  // Condition variable
    bool                active;       // Thread has work
    int                 id;           // Thread ID (0 to num_threads-1)
    struct s_data       *data;        // Pointer to shared data
} t_thread;
```

**Purpose:** Manages worker threads for parallel rendering.

**Work Distribution:**
- Thread 0 renders rows: 0, N, 2N, 3N, ...
- Thread 1 renders rows: 1, N+1, 2N+1, 3N+1, ...
- Thread k renders rows: k, N+k, 2N+k, 3N+k, ...

Where N = number of threads.

---

### 12. Network Message (t_msg_header)

**Location:** `inc/server.h`

```c
typedef enum e_msg_types {
    MSG_SCENE_DATA,      // Scene file content
    MSG_RENDER_TILE,     // Tile rendering assignment
    MSG_TILE_COMPLETE,   // Rendered tile result
    MSG_WORKER_READY,    // Worker initialization done
    MSG_ERROR,           // Error notification
    MSG_SCENE_FILE,      // Scene file transmission
    MSG_SHUTDOWN,        // Graceful shutdown
    MSG_SETTINGS,        // Rendering settings
    MSG_UPDATE           // Camera/settings update
} t_msg_types;

typedef struct s_msg_header {
    uint32_t msg_type;      // Message type (from enum)
    uint32_t payload_size;  // Payload size in bytes
} t_msg_header;
```

**Purpose:** Network protocol for master-worker communication.

**Message Flow Example:**
```
Master → Worker: MSG_SETTINGS (rendering configuration)
Master → Worker: MSG_SCENE_FILE (scene description)
Worker → Master: MSG_WORKER_READY (ready to render)
Master → Worker: MSG_RENDER_TILE (tile assignment)
Worker → Master: MSG_TILE_COMPLETE (rendered pixels)
```

---

### 13. Main Data Structure (t_data)

**Location:** `inc/mini_rt.h`

```c
typedef struct s_data {
    // Window and graphics
    mlx_t           *mlx;
    mlx_image_t     *img;
    int             width;
    int             height;

    // Scene elements
    t_camera        camera;
    t_object        *objects;        // Linked list
    t_light         *lights;         // Linked list
    t_rgb           ambient_color;
    double          ambient_intensity;

    // Acceleration structure
    t_bvh_node      *bvh_root;

    // Rendering settings
    t_settings      settings;

    // Multi-threading
    t_thread        *threads;
    int             num_threads;
    int             threads_done;

    // Network rendering (master mode)
    int             master_socket;
    t_worker        *workers;
    int             num_workers;
    t_tile_queue    *tile_queue;
    bool            start_render;

    // Network rendering (worker mode)
    int             worker_socket;
    bool            is_worker;
} t_data;
```

**Purpose:** Central data structure containing all application state.

---

## Module Documentation

### Main Entry Point

**File:** `src/main.c`

**Responsibilities:**
- Parse command-line arguments
- Route to appropriate mode (local/master/worker)
- Initialize graphics and scene
- Start main loop

**Key Functions:**

#### `int main(int argc, char **argv)`

Entry point for the application.

**Logic Flow:**
```c
int main(int argc, char **argv) {
    // 1. Parse command line
    if (argc < 2)
        print_usage();

    // 2. Check mode flags
    if (has_flag(argv, "--master"))
        return master_mode(argv);
    else if (has_flag(argv, "--worker"))
        return worker_mode(argv);
    else
        return local_mode(argv);
}
```

**Local Mode:**
```c
int local_mode(char **argv) {
    t_data data;

    // Parse scene file
    if (!parse_scene(argv[1], &data))
        return error("Invalid scene file");

    // Build BVH
    if (data.settings.use_bvh)
        data.bvh_root = build_bvh(data.objects);

    // Initialize graphics
    data.mlx = mlx_init(data.width, data.height, "miniRT", true);
    data.img = mlx_new_image(data.mlx, data.width, data.height);

    // Initialize camera
    init_camera(&data);

    // Setup threads
    init_threads(&data);

    // Setup event handlers
    mlx_key_hook(data.mlx, key_hook, &data);

    // Initial render
    render_with_mt(&data);

    // Main loop
    mlx_loop(data.mlx);

    // Cleanup
    cleanup_data(&data);
    return 0;
}
```

---

### Ray Tracing Core

**Directory:** `src/logic/`

This module contains the core ray tracing algorithm.

---

#### Ray Color Calculation

**File:** `src/logic/ray.c`

**Function:** `t_rgb ray_color(t_ray *ray, t_data *data, int depth)`

**Purpose:** Recursively traces a ray through the scene and calculates its color.

**Algorithm:**
```c
t_rgb ray_color(t_ray *ray, t_data *data, int depth) {
    // 1. Check recursion depth
    if (depth >= data->settings.depth)
        return rgb_zero();  // Too deep - return black

    // 2. Find nearest intersection
    t_hit_record rec;
    bool hit = hit_any(data, ray, 0.001, INFINITY, &rec);

    // 3. No hit - return sky color
    if (!hit) {
        double t = 0.5 * (ray->direction.y + 1.0);
        return rgb_lerp(&sky_color_1, &sky_color_2, t);
    }

    // 4. Material scattering
    t_ray scattered;
    t_rgb attenuation;
    bool scattered_ok = false;

    if (rec.material->type == LAMBERTIAN)
        scattered_ok = lambertian_scatter(ray, &rec, &attenuation, &scattered);
    else if (rec.material->type == METAL)
        scattered_ok = metal_scatter(ray, &rec, &attenuation, &scattered);
    else if (rec.material->type == DIELECTRIC)
        scattered_ok = dielectric_scatter(ray, &rec, &attenuation, &scattered);

    if (!scattered_ok)
        return rgb_zero();

    // 5. Calculate lighting
    t_rgb light_contribution = calculate_lighting(data, &rec);

    // 6. Recursive ray trace
    t_rgb scattered_color = ray_color(&scattered, data, depth + 1);

    // 7. Combine colors
    t_rgb result = rgb_modulate(&attenuation, &scattered_color);
    result = rgb_add(&result, &light_contribution);

    return result;
}
```

**Key Points:**
- **Depth limiting**: Prevents infinite recursion
- **Early termination**: No hit → sky color
- **Material dispatch**: Different logic per material type
- **Recursive bounces**: Simulates global illumination
- **Lighting**: Direct lighting from light sources

---

#### Material Scattering

**Files:**
- `src/logic/lambertian.c`
- `src/logic/metal.c`
- `src/logic/dielectric.c`

**Lambertian (Diffuse):**

```c
bool lambertian_scatter(t_ray *ray, t_hit_record *rec,
                        t_rgb *attenuation, t_ray *scattered) {
    // Random scatter in hemisphere
    t_vec3 scatter_direction = random_on_hemisphere(&rec->normal);

    // Handle degenerate case
    if (vec_near_zero(&scatter_direction))
        scatter_direction = rec->normal;

    // Generate scattered ray
    scattered->origin = rec->point;
    scattered->direction = vec_normalize(&scatter_direction);

    // Attenuation = surface color
    *attenuation = rec->material->albedo;

    return true;
}
```

**Metal (Specular):**

```c
bool metal_scatter(t_ray *ray, t_hit_record *rec,
                   t_rgb *attenuation, t_ray *scattered) {
    // Perfect reflection
    t_vec3 reflected = vec_reflect(&ray->direction, &rec->normal);

    // Add fuzz (roughness)
    if (rec->material->param > 0.0) {
        t_vec3 fuzz = vec_multiply(&random_unit_vec3(), rec->material->param);
        reflected = vec_add(&reflected, &fuzz);
    }

    scattered->origin = rec->point;
    scattered->direction = vec_normalize(&reflected);
    *attenuation = rec->material->albedo;

    // Absorbed if scattered into surface
    return vec_dot(&scattered->direction, &rec->normal) > 0;
}
```

**Dielectric (Glass):**

```c
bool dielectric_scatter(t_ray *ray, t_hit_record *rec,
                        t_rgb *attenuation, t_ray *scattered) {
    *attenuation = (t_rgb){1.0, 1.0, 1.0};  // No color absorption

    double refraction_ratio = rec->front_face
        ? (1.0 / rec->material->param)
        : rec->material->param;

    double cos_theta = fmin(vec_dot(&vec_negate(&ray->direction),
                                    &rec->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Total internal reflection
    bool cannot_refract = refraction_ratio * sin_theta > 1.0;

    t_vec3 direction;
    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
        // Reflect
        direction = vec_reflect(&ray->direction, &rec->normal);
    } else {
        // Refract
        direction = vec_refract(&ray->direction, &rec->normal, refraction_ratio);
    }

    scattered->origin = rec->point;
    scattered->direction = direction;
    return true;
}

// Schlick's approximation for reflectance
static double reflectance(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
```

---

#### Intersection Tests

**Files:** `src/logic/hit_*.c`

**Sphere Intersection (`src/logic/hit_sphere.c`):**

```c
bool hit_sphere(t_object *obj, t_ray *ray, t_hit_record *rec) {
    t_sphere *sphere = (t_sphere *)obj->data;

    // Ray-sphere intersection (quadratic equation)
    t_vec3 oc = vec_sub(&ray->origin, &sphere->center);
    double a = vec_dot(&ray->direction, &ray->direction);
    double half_b = vec_dot(&oc, &ray->direction);
    double c = vec_dot(&oc, &oc) - sphere->radius * sphere->radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return false;  // No intersection

    // Find nearest root in acceptable range
    double sqrtd = sqrt(discriminant);
    double root = (-half_b - sqrtd) / a;
    if (root < 0.001 || root > rec->t) {
        root = (-half_b + sqrtd) / a;
        if (root < 0.001 || root > rec->t)
            return false;
    }

    // Fill hit record
    rec->t = root;
    rec->point = ray_at(ray, root);
    t_vec3 outward_normal = vec_sub(&rec->point, &sphere->center);
    outward_normal = vec_divide(&outward_normal, sphere->radius);
    set_face_normal(rec, ray, &outward_normal);
    rec->material = &obj->material;

    return true;
}
```

**Plane Intersection (`src/logic/hit_plane.c`):**

```c
bool hit_plane(t_object *obj, t_ray *ray, t_hit_record *rec) {
    t_plane *plane = (t_plane *)obj->data;

    double denom = vec_dot(&plane->normal, &ray->direction);

    // Parallel to plane
    if (fabs(denom) < 1e-6)
        return false;

    t_vec3 p0_origin = vec_sub(&plane->point, &ray->origin);
    double t = vec_dot(&p0_origin, &plane->normal) / denom;

    if (t < 0.001 || t > rec->t)
        return false;

    rec->t = t;
    rec->point = ray_at(ray, t);
    set_face_normal(rec, ray, &plane->normal);
    rec->material = &obj->material;

    return true;
}
```

**Cylinder Intersection (`src/logic/hit_tower.c`):**

Cylinder intersection is more complex, involving:
1. Infinite cylinder body intersection (quadratic)
2. Cap intersections (two circles)
3. Height bounds checking

```c
bool hit_cylinder(t_object *obj, t_ray *ray, t_hit_record *rec) {
    t_cylinder *cyl = (t_cylinder *)obj->data;

    // 1. Transform ray to cylinder space
    // 2. Solve quadratic for infinite cylinder
    // 3. Check height bounds
    // 4. Check cap intersections
    // 5. Return closest valid hit

    // (Implementation details in src/logic/hit_tower.c)
}
```

---

#### Anti-Aliasing

**File:** `src/logic/monte_carlo_aa.c`

**Function:** `uint32_t monte_carlo_aa(t_data *data, int i, int j)`

**Purpose:** Performs Monte Carlo anti-aliasing by sampling multiple rays per pixel.

```c
uint32_t monte_carlo_aa(t_data *data, int i, int j) {
    t_rgb color_sum = {0, 0, 0};

    // Adaptive sampling based on pixel complexity
    int samples = compute_sample_count(data, i, j);

    for (int s = 0; s < samples; s++) {
        // Random offset within pixel
        double u = ((double)i + random_double()) / (data->width - 1);
        double v = ((double)j + random_double()) / (data->height - 1);

        // Generate ray through pixel
        t_ray ray = get_ray(data, u, v);

        // Trace ray
        t_rgb sample_color = ray_color(&ray, data, 0);
        color_sum = rgb_add(&color_sum, &sample_color);
    }

    // Average samples
    t_rgb final_color = rgb_divide(&color_sum, samples);

    // Gamma correction
    final_color.r = sqrt(final_color.r);
    final_color.g = sqrt(final_color.g);
    final_color.b = sqrt(final_color.b);

    return rgb_to_uint32(&final_color);
}
```

**Without AA:**

```c
uint32_t without_aa(t_data *data, int i, int j) {
    // Single ray through pixel center
    double u = (double)i / (data->width - 1);
    double v = (double)j / (data->height - 1);

    t_ray ray = get_ray(data, u, v);
    t_rgb color = ray_color(&ray, data, 0);

    // Gamma correction
    color.r = sqrt(color.r);
    color.g = sqrt(color.g);
    color.b = sqrt(color.b);

    return rgb_to_uint32(&color);
}
```

---

#### BVH Acceleration

**Files:**
- `src/logic/bvh_build.c` - Construction
- `src/logic/bvh_hit.c` - Traversal
- `src/logic/bounds.c` - Bounding box utilities
- `src/logic/aabb.c` - Axis-aligned bounding box tests

**BVH Construction:**

```c
t_bvh_node *build_bvh(t_object *objects) {
    // 1. Count objects
    int count = count_objects(objects);
    t_object **obj_array = objects_to_array(objects, count);

    // 2. Recursive build
    return build_bvh_recursive(obj_array, 0, count);
}

t_bvh_node *build_bvh_recursive(t_object **objects, int start, int end) {
    t_bvh_node *node = malloc(sizeof(t_bvh_node));

    int count = end - start;

    // Base case - leaf node
    if (count <= BVH_LEAF_THRESHOLD) {
        node->objects = &objects[start];
        node->object_count = count;
        node->left = NULL;
        node->right = NULL;
        node->aabb = compute_bounding_box(objects, start, end);
        return node;
    }

    // Recursive case - split node
    // 1. Find longest axis
    t_aabb bbox = compute_bounding_box(objects, start, end);
    int axis = longest_axis(&bbox);

    // 2. Sort objects along axis
    qsort(&objects[start], count, sizeof(t_object *),
          comparator[axis]);

    // 3. Split at median
    int mid = start + count / 2;

    // 4. Recursively build children
    node->left = build_bvh_recursive(objects, start, mid);
    node->right = build_bvh_recursive(objects, mid, end);
    node->objects = NULL;
    node->object_count = 0;

    // 5. Compute combined bounding box
    node->aabb = aabb_union(&node->left->aabb, &node->right->aabb);

    return node;
}
```

**BVH Traversal:**

```c
bool bvh_hit(t_bvh_node *node, t_ray *ray, t_hit_record *rec) {
    // Early exit - ray misses bounding box
    if (!aabb_hit(&node->aabb, ray, 0.001, rec->t))
        return false;

    // Leaf node - test all objects
    if (node->left == NULL) {
        bool hit_anything = false;
        for (int i = 0; i < node->object_count; i++) {
            if (hit_object(node->objects[i], ray, rec))
                hit_anything = true;  // rec->t updated to closest
        }
        return hit_anything;
    }

    // Interior node - test both children
    bool hit_left = bvh_hit(node->left, ray, rec);
    bool hit_right = bvh_hit(node->right, ray, rec);

    return hit_left || hit_right;
}
```

**AABB Intersection:**

```c
bool aabb_hit(t_aabb *box, t_ray *ray, double t_min, double t_max) {
    // Slab method
    for (int a = 0; a < 3; a++) {
        double inv_d = 1.0 / ray->direction.e[a];
        double t0 = (box->min.e[a] - ray->origin.e[a]) * inv_d;
        double t1 = (box->max.e[a] - ray->origin.e[a]) * inv_d;

        if (inv_d < 0.0)
            swap(&t0, &t1);

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
            return false;
    }
    return true;
}
```

---

#### Rendering Loop

**File:** `src/logic/render.c`

**Function:** `int render_with_mt(t_data *data)`

**Purpose:** Multi-threaded rendering of the entire frame.

```c
int render_with_mt(t_data *data) {
    int start_time = get_time_in_ms();

    // Reset completion counter
    data->threads_done = 0;

    // Wake up all threads
    for (int i = 0; i < data->num_threads; i++) {
        pthread_mutex_lock(&data->threads[i].active_mutex);
        data->threads[i].active = true;
        pthread_cond_signal(&data->threads[i].active_cond);
        pthread_mutex_unlock(&data->threads[i].active_mutex);
    }

    // Wait for all threads to complete
    while (data->threads_done < data->num_threads) {
        usleep(1000);  // Sleep 1ms
    }

    // Calculate and display FPS
    int end_time = get_time_in_ms();
    double fps = 1000.0 / (end_time - start_time);
    printf("FPS: %.2f\r", fps);
    fflush(stdout);

    return 0;
}
```

**Thread Job:**

```c
void *thread_job(void *arg) {
    t_thread *thread = (t_thread *)arg;
    t_data *data = thread->data;

    while (true) {
        // Wait for work
        pthread_mutex_lock(&thread->active_mutex);
        while (!thread->active) {
            pthread_cond_wait(&thread->active_cond, &thread->active_mutex);
        }
        pthread_mutex_unlock(&thread->active_mutex);

        // Check for exit signal
        if (data->shutdown)
            break;

        // Render assigned rows (stripe pattern)
        for (int j = thread->id; j < data->height; j += data->num_threads) {
            for (int i = 0; i < data->width; i++) {
                uint32_t color;
                if (data->settings.aa_state)
                    color = monte_carlo_aa(data, i, j);
                else
                    color = without_aa(data, i, j);

                // Write pixel to framebuffer
                mlx_put_pixel(data->img, i, data->height - j - 1, color);
            }
        }

        // Mark inactive
        pthread_mutex_lock(&thread->active_mutex);
        thread->active = false;
        pthread_mutex_unlock(&thread->active_mutex);

        // Increment completion counter
        __atomic_add_fetch(&data->threads_done, 1, __ATOMIC_SEQ_CST);
    }

    return NULL;
}
```

---

### Scene Parsing

**Directory:** `src/parsing/`

Responsible for reading and validating `.rt` scene files.

---

#### Main Parser

**File:** `src/parsing/parsing.c`

**Function:** `bool parse_scene(const char *filename, t_data *data)`

**Logic:**

```c
bool parse_scene(const char *filename, t_data *data) {
    // 1. Validate file extension
    if (!ends_with(filename, ".rt"))
        return error("File must have .rt extension");

    // 2. Open file
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
        return error("Cannot open file");

    // 3. Initialize data
    init_data(data);

    // 4. Parse line by line
    char *line;
    int line_num = 0;
    while ((line = get_next_line(fd)) != NULL) {
        line_num++;

        // Skip empty lines and comments
        if (is_empty_or_comment(line)) {
            free(line);
            continue;
        }

        // Tokenize line
        char **tokens = ft_split(line, ' ');
        if (!tokens) {
            free(line);
            return error("Memory allocation failed");
        }

        // Route to appropriate parser
        bool success = false;
        if (strcmp(tokens[0], "A") == 0)
            success = parse_ambient(tokens, data);
        else if (strcmp(tokens[0], "C") == 0)
            success = parse_camera(tokens, data);
        else if (strcmp(tokens[0], "L") == 0)
            success = parse_light(tokens, data);
        else if (strcmp(tokens[0], "sp") == 0)
            success = parse_sphere(tokens, data);
        else if (strcmp(tokens[0], "pl") == 0)
            success = parse_plane(tokens, data);
        else if (strcmp(tokens[0], "cy") == 0)
            success = parse_cylinder(tokens, data);
        else if (strcmp(tokens[0], "co") == 0)
            success = parse_cone(tokens, data);
        else if (strcmp(tokens[0], "py") == 0)
            success = parse_pyramid(tokens, data);
        else if (strcmp(tokens[0], "tr") == 0)
            success = parse_triangle(tokens, data);
        else
            return error_line("Unknown element type", line_num);

        free_tokens(tokens);
        free(line);

        if (!success)
            return error_line("Parse error", line_num);
    }

    close(fd);

    // 5. Validate required elements
    if (!data->has_ambient)
        return error("Missing ambient light (A)");
    if (!data->has_camera)
        return error("Missing camera (C)");
    if (data->lights == NULL)
        return error("Missing at least one light (L)");

    return true;
}
```

---

#### Element Parsers

**Ambient Light:**

```c
bool parse_ambient(char **tokens, t_data *data) {
    // Syntax: A <intensity> <R,G,B>
    if (data->has_ambient)
        return error("Duplicate ambient light");

    if (count_tokens(tokens) != 3)
        return error("Ambient: expected 2 parameters");

    // Parse intensity
    data->ambient_intensity = ft_atof(tokens[1]);
    if (data->ambient_intensity < 0.0 || data->ambient_intensity > 1.0)
        return error("Ambient intensity must be in range [0.0, 1.0]");

    // Parse color
    if (!parse_rgb(tokens[2], &data->ambient_color))
        return error("Invalid ambient color");

    data->has_ambient = true;
    return true;
}
```

**Camera:**

```c
bool parse_camera(char **tokens, t_data *data) {
    // Syntax: C <position> <orientation> <FOV>
    if (data->has_camera)
        return error("Duplicate camera");

    if (count_tokens(tokens) != 4)
        return error("Camera: expected 3 parameters");

    // Parse position
    if (!parse_vec3(tokens[1], &data->camera.position))
        return error("Invalid camera position");

    // Parse orientation
    if (!parse_vec3(tokens[2], &data->camera.orientation))
        return error("Invalid camera orientation");

    // Validate normalized
    if (!is_normalized(&data->camera.orientation))
        return error("Camera orientation must be normalized");

    // Parse FOV
    data->camera.fov = ft_atof(tokens[3]);
    if (data->camera.fov < 0.0 || data->camera.fov > 180.0)
        return error("FOV must be in range [0, 180]");

    data->has_camera = true;
    return true;
}
```

**Sphere:**

```c
bool parse_sphere(char **tokens, t_data *data) {
    // Syntax: sp <center> <diameter> <R,G,B> [material]
    if (count_tokens(tokens) < 4)
        return error("Sphere: expected at least 3 parameters");

    // Allocate sphere
    t_sphere *sphere = malloc(sizeof(t_sphere));
    if (!sphere)
        return error("Memory allocation failed");

    // Parse center
    if (!parse_vec3(tokens[1], &sphere->center)) {
        free(sphere);
        return error("Invalid sphere center");
    }

    // Parse diameter
    double diameter = ft_atof(tokens[2]);
    if (diameter <= 0.0) {
        free(sphere);
        return error("Sphere diameter must be positive");
    }
    sphere->radius = diameter / 2.0;

    // Parse color
    if (!parse_rgb(tokens[3], &sphere->color)) {
        free(sphere);
        return error("Invalid sphere color");
    }

    // Create object
    t_object *obj = malloc(sizeof(t_object));
    obj->type = SPHERE;
    obj->data = sphere;

    // Parse material (optional)
    if (tokens[4] != NULL)
        parse_material(tokens[4], &obj->material);
    else
        obj->material = default_lambertian(&sphere->color);

    // Add to list
    obj->next = data->objects;
    data->objects = obj;

    return true;
}
```

---

#### Material Parsing

**File:** `src/parsing/mat_token.c`

```c
bool parse_material(const char *mat_str, t_material *material) {
    // Syntax: mat:<type>:<param1>:<param2>:...

    if (!starts_with(mat_str, "mat:"))
        return false;  // No material specified

    char **parts = ft_split(mat_str + 4, ':');  // Skip "mat:"
    if (!parts)
        return false;

    bool success = false;

    if (strcmp(parts[0], "lambertian") == 0) {
        material->type = LAMBERTIAN;
        material->param = 0.0;
        success = true;
    }
    else if (strcmp(parts[0], "metal") == 0) {
        material->type = METAL;
        if (parts[1] != NULL) {
            material->param = ft_atof(parts[1]);  // Roughness
            if (material->param < 0.0 || material->param > 1.0)
                success = false;
            else
                success = true;
        }
    }
    else if (strcmp(parts[0], "dielectric") == 0) {
        material->type = DIELECTRIC;
        if (parts[1] != NULL) {
            material->param = ft_atof(parts[1]);  // IOR
            if (material->param < 1.0)
                success = false;
            else
                success = true;
        }
    }

    // Check for bump mapping
    for (int i = 0; parts[i] != NULL; i++) {
        if (strcmp(parts[i], "bump") == 0 && parts[i+1] != NULL) {
            double scale = ft_atof(parts[i+1]);
            material->bump = create_bump_function(scale);
            break;
        }
    }

    free_tokens(parts);
    return success;
}
```

---

### Network Rendering

**Directory:** `src/server/`

Implements distributed rendering via master-worker architecture.

---

#### Master Mode

**File:** `src/server/master.c`

**Function:** `int master_mode(char **argv)`

```c
int master_mode(char **argv) {
    t_data data;

    // 1. Parse scene file
    if (!parse_scene(argv[scene_index], &data))
        return error("Parse failed");

    // 2. Build tile queue
    data.tile_queue = init_tile_queue(data.width, data.height);

    // 3. Initialize MLX
    data.mlx = mlx_init(data.width, data.height, "miniRT Master", true);
    data.img = mlx_new_image(data.mlx, data.width, data.height);

    // 4. Setup network
    int port = get_port(argv);
    data.master_socket = setup_listen_socket(port);
    if (data.master_socket < 0)
        return error("Socket setup failed");

    printf("Master listening on port %d\n", port);
    printf("Waiting for workers to connect...\n");

    // 5. Start accept thread
    pthread_t accept_thread;
    pthread_create(&accept_thread, NULL, accept_workers, &data);

    // 6. Wait for user to start rendering
    printf("Press ENTER to start rendering\n");
    getchar();
    data.start_render = true;

    // 7. Start worker threads (one per connected worker)
    for (int i = 0; i < data.num_workers; i++) {
        pthread_create(&data.workers[i].thread, NULL,
                      worker_thread_func, &data.workers[i]);
    }

    // 8. Setup event handlers
    mlx_key_hook(data.mlx, key_hook_master, &data);

    // 9. Main loop
    mlx_loop(data.mlx);

    // 10. Cleanup
    broadcast_shutdown(&data);
    cleanup_master(&data);

    return 0;
}
```

**Accept Worker Thread:**

```c
void *accept_workers(void *arg) {
    t_data *data = (t_data *)arg;

    while (!data->shutdown) {
        // Accept connection
        struct sockaddr_in worker_addr;
        socklen_t addr_len = sizeof(worker_addr);
        int worker_socket = accept(data->master_socket,
                                   (struct sockaddr *)&worker_addr,
                                   &addr_len);

        if (worker_socket < 0)
            continue;

        printf("Worker connected from %s\n",
               inet_ntoa(worker_addr.sin_addr));

        // Register worker
        t_worker *worker = register_worker(data, worker_socket);

        // Send settings
        send_settings(worker_socket, &data->settings);

        // Send scene file
        send_scene_file(worker_socket, data->scene_filename);

        // Wait for worker ready
        t_msg_header header;
        recv_header(worker_socket, &header);
        if (header.msg_type != MSG_WORKER_READY) {
            printf("Worker handshake failed\n");
            close(worker_socket);
            continue;
        }

        printf("Worker %d ready\n", data->num_workers);
        data->num_workers++;
    }

    return NULL;
}
```

**Worker Thread (Master Side):**

```c
void *worker_thread_func(void *arg) {
    t_worker *worker = (t_worker *)arg;
    t_data *data = worker->data;

    // Wait for rendering to start
    while (!data->start_render && !data->shutdown)
        usleep(10000);

    while (!data->shutdown) {
        // Get next tile from queue
        t_tile tile;
        if (!queue_next_job(data->tile_queue, &tile)) {
            // No more tiles
            usleep(10000);
            continue;
        }

        // Send tile assignment to worker
        send_tile_assignment(worker->socket, &tile);

        // Receive rendered tile
        uint32_t *pixels = recv_tile_result(worker->socket, &tile);
        if (!pixels) {
            // Worker disconnected
            printf("Worker %d disconnected\n", worker->id);
            queue_requeue_tile(data->tile_queue, &tile);
            break;
        }

        // Composite tile into framebuffer
        for (int y = 0; y < tile.height; y++) {
            for (int x = 0; x < tile.width; x++) {
                int img_x = tile.x + x;
                int img_y = tile.y + y;
                uint32_t color = pixels[y * tile.width + x];
                mlx_put_pixel(data->img, img_x, img_y, color);
            }
        }

        free(pixels);
    }

    return NULL;
}
```

---

#### Worker Mode

**File:** `src/server/worker.c`

**Function:** `int worker_mode(char **argv)`

```c
int worker_mode(char **argv) {
    t_data data;
    data.is_worker = true;

    // 1. Connect to master
    char *master_ip = get_master_ip(argv);
    int port = get_port(argv);

    data.worker_socket = connect_to_master(master_ip, port);
    if (data.worker_socket < 0)
        return error("Connection failed");

    printf("Connected to master %s:%d\n", master_ip, port);

    // 2. Receive settings
    if (!recv_settings(data.worker_socket, &data.settings))
        return error("Failed to receive settings");

    // 3. Receive scene file
    char *scene_content = recv_scene_file(data.worker_socket);
    if (!scene_content)
        return error("Failed to receive scene");

    // 4. Write scene to temporary file
    int fd = open("worker_scene.rt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, scene_content, strlen(scene_content));
    close(fd);
    free(scene_content);

    // 5. Parse scene
    if (!parse_scene("worker_scene.rt", &data))
        return error("Parse failed");

    // 6. Build BVH
    if (data.settings.use_bvh)
        data.bvh_root = build_bvh(data.objects);

    // 7. Initialize camera
    init_camera(&data);

    // 8. Setup threads
    init_threads_worker(&data);

    // 9. Send ready message
    send_worker_ready(data.worker_socket);

    printf("Worker ready, waiting for tiles...\n");

    // 10. Main loop
    while (true) {
        // Receive message header
        t_msg_header header;
        if (!recv_header(data.worker_socket, &header))
            break;  // Connection lost

        if (header.msg_type == MSG_SHUTDOWN) {
            printf("Shutdown received\n");
            break;
        }
        else if (header.msg_type == MSG_UPDATE) {
            // Camera update
            t_camera_update update;
            recv_camera_update(data.worker_socket, &update);
            apply_camera_update(&data, &update);
        }
        else if (header.msg_type == MSG_RENDER_TILE) {
            // Tile assignment
            t_tile tile;
            recv_tile_assignment(data.worker_socket, &tile);

            // Render tile
            uint32_t *pixels = render_tile_worker(&data, &tile);

            // Send result
            send_tile_result(data.worker_socket, &tile, pixels);

            free(pixels);
        }
    }

    // 11. Cleanup
    cleanup_data(&data);
    close(data.worker_socket);

    return 0;
}
```

**Tile Rendering (Worker):**

```c
uint32_t *render_tile_worker(t_data *data, t_tile *tile) {
    // Allocate pixel buffer
    uint32_t *pixels = malloc(tile->width * tile->height * sizeof(uint32_t));
    if (!pixels)
        return NULL;

    // Reset thread completion counter
    data->threads_done = 0;
    data->current_tile = tile;
    data->tile_pixels = pixels;

    // Wake up threads
    for (int i = 0; i < data->num_threads; i++) {
        pthread_mutex_lock(&data->threads[i].active_mutex);
        data->threads[i].active = true;
        pthread_cond_signal(&data->threads[i].active_cond);
        pthread_mutex_unlock(&data->threads[i].active_mutex);
    }

    // Wait for completion
    while (data->threads_done < data->num_threads)
        usleep(100);

    return pixels;
}

void *thread_job_worker(void *arg) {
    t_thread *thread = (t_thread *)arg;
    t_data *data = thread->data;

    while (true) {
        // Wait for work
        pthread_mutex_lock(&thread->active_mutex);
        while (!thread->active)
            pthread_cond_wait(&thread->active_cond, &thread->active_mutex);
        pthread_mutex_unlock(&thread->active_mutex);

        if (data->shutdown)
            break;

        t_tile *tile = data->current_tile;

        // Render assigned rows of tile
        for (int local_y = thread->id; local_y < tile->height;
             local_y += data->num_threads) {

            int global_y = tile->y + local_y;

            for (int local_x = 0; local_x < tile->width; local_x++) {
                int global_x = tile->x + local_x;

                uint32_t color;
                if (data->settings.aa_state)
                    color = monte_carlo_aa(data, global_x, global_y);
                else
                    color = without_aa(data, global_x, global_y);

                data->tile_pixels[local_y * tile->width + local_x] = color;
            }
        }

        // Mark inactive
        pthread_mutex_lock(&thread->active_mutex);
        thread->active = false;
        pthread_mutex_unlock(&thread->active_mutex);

        __atomic_add_fetch(&data->threads_done, 1, __ATOMIC_SEQ_CST);
    }

    return NULL;
}
```

---

#### Tile Queue

**File:** `src/server/queue.c`

```c
typedef struct s_tile {
    int x, y;           // Top-left corner
    int width, height;  // Dimensions
} t_tile;

typedef struct s_tile_queue {
    t_tile  *tiles;      // Array of tiles
    int     total;       // Total tile count
    int     current;     // Next tile index
    pthread_mutex_t mutex;
} t_tile_queue;

t_tile_queue *init_tile_queue(int img_width, int img_height) {
    t_tile_queue *queue = malloc(sizeof(t_tile_queue));

    // Calculate tile grid dimensions
    int tiles_x = (img_width + TILE_SIZE - 1) / TILE_SIZE;
    int tiles_y = (img_height + TILE_SIZE - 1) / TILE_SIZE;
    queue->total = tiles_x * tiles_y;

    // Allocate tiles
    queue->tiles = malloc(sizeof(t_tile) * queue->total);

    // Fill tiles
    int idx = 0;
    for (int ty = 0; ty < tiles_y; ty++) {
        for (int tx = 0; tx < tiles_x; tx++) {
            queue->tiles[idx].x = tx * TILE_SIZE;
            queue->tiles[idx].y = ty * TILE_SIZE;
            queue->tiles[idx].width = min(TILE_SIZE, img_width - queue->tiles[idx].x);
            queue->tiles[idx].height = min(TILE_SIZE, img_height - queue->tiles[idx].y);
            idx++;
        }
    }

    queue->current = 0;
    pthread_mutex_init(&queue->mutex, NULL);

    return queue;
}

bool queue_next_job(t_tile_queue *queue, t_tile *out_tile) {
    pthread_mutex_lock(&queue->mutex);

    if (queue->current >= queue->total) {
        pthread_mutex_unlock(&queue->mutex);
        return false;  // No more tiles
    }

    *out_tile = queue->tiles[queue->current];
    queue->current++;

    pthread_mutex_unlock(&queue->mutex);
    return true;
}

void queue_reset(t_tile_queue *queue) {
    pthread_mutex_lock(&queue->mutex);
    queue->current = 0;
    pthread_mutex_unlock(&queue->mutex);
}
```

---

#### Network Protocol

**Message Sending (src/server/send/):**

```c
// Send header
bool send_header(int socket, uint32_t msg_type, uint32_t payload_size) {
    t_msg_header header;
    header.msg_type = htonl(msg_type);      // Host to network byte order
    header.payload_size = htonl(payload_size);

    return send_all(socket, &header, sizeof(header));
}

// Send all bytes (handles partial sends)
bool send_all(int socket, const void *data, size_t size) {
    size_t sent = 0;
    while (sent < size) {
        ssize_t n = send(socket, (char *)data + sent, size - sent, 0);
        if (n <= 0)
            return false;  // Error or connection closed
        sent += n;
    }
    return true;
}

// Send settings
bool send_settings(int socket, t_settings *settings) {
    if (!send_header(socket, MSG_SETTINGS, sizeof(t_settings)))
        return false;
    return send_all(socket, settings, sizeof(t_settings));
}

// Send tile assignment
bool send_tile_assignment(int socket, t_tile *tile) {
    if (!send_header(socket, MSG_RENDER_TILE, sizeof(t_tile)))
        return false;
    return send_all(socket, tile, sizeof(t_tile));
}

// Send tile result (pixels)
bool send_tile_result(int socket, t_tile *tile, uint32_t *pixels) {
    size_t pixel_count = tile->width * tile->height;
    size_t payload_size = sizeof(t_tile) + pixel_count * sizeof(uint32_t);

    if (!send_header(socket, MSG_TILE_COMPLETE, payload_size))
        return false;
    if (!send_all(socket, tile, sizeof(t_tile)))
        return false;
    return send_all(socket, pixels, pixel_count * sizeof(uint32_t));
}
```

**Message Receiving (src/server/recive/):**

```c
// Receive header
bool recv_header(int socket, t_msg_header *header) {
    if (!recv_all(socket, header, sizeof(t_msg_header)))
        return false;

    // Network to host byte order
    header->msg_type = ntohl(header->msg_type);
    header->payload_size = ntohl(header->payload_size);

    return true;
}

// Receive all bytes
bool recv_all(int socket, void *data, size_t size) {
    size_t received = 0;
    while (received < size) {
        ssize_t n = recv(socket, (char *)data + received, size - received, 0);
        if (n <= 0)
            return false;
        received += n;
    }
    return true;
}

// Receive tile result
uint32_t *recv_tile_result(int socket, t_tile *tile_out) {
    // Receive tile info
    if (!recv_all(socket, tile_out, sizeof(t_tile)))
        return NULL;

    // Allocate pixel buffer
    size_t pixel_count = tile_out->width * tile_out->height;
    uint32_t *pixels = malloc(pixel_count * sizeof(uint32_t));
    if (!pixels)
        return NULL;

    // Receive pixels
    if (!recv_all(socket, pixels, pixel_count * sizeof(uint32_t))) {
        free(pixels);
        return NULL;
    }

    return pixels;
}
```

---

### Math Library

**Directory:** `src/math/`

Custom vector and color math implementations.

---

#### Vector Operations

**Files:** `src/math/vec_*.c`

```c
// Vector addition
t_vec3 vec_add(const t_vec3 *a, const t_vec3 *b) {
    return (t_vec3){a->x + b->x, a->y + b->y, a->z + b->z};
}

// Vector subtraction
t_vec3 vec_sub(const t_vec3 *a, const t_vec3 *b) {
    return (t_vec3){a->x - b->x, a->y - b->y, a->z - b->z};
}

// Scalar multiplication
t_vec3 vec_multiply(const t_vec3 *v, double t) {
    return (t_vec3){v->x * t, v->y * t, v->z * t};
}

// Scalar division
t_vec3 vec_divide(const t_vec3 *v, double t) {
    return vec_multiply(v, 1.0 / t);
}

// Dot product
double vec_dot(const t_vec3 *a, const t_vec3 *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

// Cross product
t_vec3 vec_cross(const t_vec3 *a, const t_vec3 *b) {
    return (t_vec3){
        a->y * b->z - a->z * b->y,
        a->z * b->x - a->x * b->z,
        a->x * b->y - a->y * b->x
    };
}

// Length (magnitude)
double vec_length(const t_vec3 *v) {
    return sqrt(vec_dot(v, v));
}

// Squared length (faster, avoids sqrt)
double vec_length_squared(const t_vec3 *v) {
    return vec_dot(v, v);
}

// Normalize (unit vector)
t_vec3 vec_normalize(const t_vec3 *v) {
    return vec_divide(v, vec_length(v));
}

// Reflection
t_vec3 vec_reflect(const t_vec3 *v, const t_vec3 *n) {
    // r = v - 2 * dot(v, n) * n
    double dot = vec_dot(v, n);
    t_vec3 temp = vec_multiply(n, 2.0 * dot);
    return vec_sub(v, &temp);
}

// Refraction (Snell's law)
t_vec3 vec_refract(const t_vec3 *uv, const t_vec3 *n, double etai_over_etat) {
    double cos_theta = fmin(vec_dot(&vec_negate(uv), n), 1.0);
    t_vec3 r_out_perp = vec_multiply(
        &vec_add(uv, &vec_multiply(n, cos_theta)),
        etai_over_etat
    );
    double perp_len_sq = vec_length_squared(&r_out_perp);
    t_vec3 r_out_parallel = vec_multiply(n, -sqrt(fabs(1.0 - perp_len_sq)));
    return vec_add(&r_out_perp, &r_out_parallel);
}
```

---

#### Random Number Generation

**File:** `src/math/random.c`

```c
// Random double in [0, 1)
double random_double(void) {
    return (double)rand() / (RAND_MAX + 1.0);
}

// Random double in [min, max)
double random_double_range(double min, double max) {
    return min + (max - min) * random_double();
}

// Random vector in unit cube
t_vec3 random_vec3(void) {
    return (t_vec3){
        random_double(),
        random_double(),
        random_double()
    };
}

// Random vector in unit sphere (rejection sampling)
t_vec3 random_in_unit_sphere(void) {
    while (true) {
        t_vec3 p = random_vec3_range(-1, 1);
        if (vec_length_squared(&p) < 1.0)
            return p;
    }
}

// Random unit vector
t_vec3 random_unit_vec3(void) {
    t_vec3 v = random_in_unit_sphere();
    return vec_normalize(&v);
}

// Random vector on hemisphere (Lambertian scattering)
t_vec3 random_on_hemisphere(const t_vec3 *normal) {
    t_vec3 on_unit_sphere = random_unit_vec3();

    // Same hemisphere as normal?
    if (vec_dot(&on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return vec_negate(&on_unit_sphere);
}
```

---

### Utilities

**Directory:** `src/utils/`

---

#### Thread Management

**File:** `src/utils/threads_init.c`

```c
int init_threads(t_data *data) {
    // Detect number of CPU cores
    data->num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if (data->num_threads < 1)
        data->num_threads = 4;  // Default fallback

    printf("Using %d threads\n", data->num_threads);

    // Allocate thread structures
    data->threads = malloc(sizeof(t_thread) * data->num_threads);
    if (!data->threads)
        return ft_error("Thread allocation failed", 1);

    // Initialize each thread
    for (int i = 0; i < data->num_threads; i++) {
        data->threads[i].id = i;
        data->threads[i].data = data;
        data->threads[i].active = false;

        pthread_mutex_init(&data->threads[i].active_mutex, NULL);
        pthread_cond_init(&data->threads[i].active_cond, NULL);

        // Create thread
        if (pthread_create(&data->threads[i].thread, NULL,
                          thread_job, &data->threads[i]) != 0) {
            return ft_error("Thread creation failed", 1);
        }
    }

    return 0;
}
```

---

#### MLX Event Hooks

**File:** `src/utils/mlx_hooks.c`

```c
void key_hook(mlx_key_data_t keydata, void *param) {
    t_data *data = (t_data *)param;

    if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
        return;

    bool need_render = false;

    // Camera movement
    t_vec3 forward, right, up;
    get_camera_vectors(data, &forward, &right, &up);

    double move_speed = 0.5;

    if (keydata.key == MLX_KEY_W) {
        t_vec3 delta = vec_multiply(&forward, move_speed);
        data->camera.position = vec_add(&data->camera.position, &delta);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_S) {
        t_vec3 delta = vec_multiply(&forward, -move_speed);
        data->camera.position = vec_add(&data->camera.position, &delta);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_A) {
        t_vec3 delta = vec_multiply(&right, -move_speed);
        data->camera.position = vec_add(&data->camera.position, &delta);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_D) {
        t_vec3 delta = vec_multiply(&right, move_speed);
        data->camera.position = vec_add(&data->camera.position, &delta);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_SPACE) {
        t_vec3 delta = vec_multiply(&up, move_speed);
        data->camera.position = vec_add(&data->camera.position, &delta);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_LEFT_SHIFT) {
        t_vec3 delta = vec_multiply(&up, -move_speed);
        data->camera.position = vec_add(&data->camera.position, &delta);
        need_render = true;
    }

    // Camera rotation
    double rot_speed = 0.1;

    if (keydata.key == MLX_KEY_UP) {
        rotate_camera_pitch(data, rot_speed);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_DOWN) {
        rotate_camera_pitch(data, -rot_speed);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_LEFT) {
        rotate_camera_yaw(data, rot_speed);
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_RIGHT) {
        rotate_camera_yaw(data, -rot_speed);
        need_render = true;
    }

    // Settings toggles
    else if (keydata.key == MLX_KEY_R) {
        data->settings.aa_state = !data->settings.aa_state;
        printf("Anti-aliasing: %s\n", data->settings.aa_state ? "ON" : "OFF");
        need_render = true;
    }
    else if (keydata.key == MLX_KEY_L) {
        data->settings.light_state = !data->settings.light_state;
        printf("Lighting: %s\n", data->settings.light_state ? "ON" : "OFF");
        need_render = true;
    }

    // Exit
    else if (keydata.key == MLX_KEY_ESCAPE) {
        mlx_close_window(data->mlx);
    }

    // Re-render if needed
    if (need_render) {
        update_camera(data);
        render_with_mt(data);
    }
}
```

---

## Function Reference

### Quick Reference Table

| Function | Location | Purpose |
|----------|----------|---------|
| `main()` | src/main.c | Application entry point |
| `ray_color()` | src/logic/ray.c | Recursive ray tracing |
| `hit_sphere()` | src/logic/hit_sphere.c | Ray-sphere intersection |
| `build_bvh()` | src/logic/bvh_build.c | BVH construction |
| `bvh_hit()` | src/logic/bvh_hit.c | BVH traversal |
| `parse_scene()` | src/parsing/parsing.c | Scene file parsing |
| `render_with_mt()` | src/logic/render.c | Multi-threaded render |
| `master_mode()` | src/server/master.c | Master mode entry |
| `worker_mode()` | src/server/worker.c | Worker mode entry |
| `init_camera()` | src/logic/camera.c | Camera initialization |
| `vec_normalize()` | src/math/vec_normalize.c | Vector normalization |

---

## Compilation and Build System

### Makefile Structure

**File:** `Makefile`

**Key Targets:**

```makefile
all: $(LIBFT) $(LIBMLX42) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBMLX42) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c inc/mini_rt.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
```

**Optimization Flags Explanation:**

- `-Ofast`: Maximum speed optimization
- `-flto`: Link-time optimization (whole program analysis)
- `-march=native`: Use CPU-specific instructions
- `-funroll-loops`: Unroll loops for better performance
- `-ftree-vectorize`: Auto-vectorization (SIMD)
- `-ffast-math`: Relaxed IEEE 754 compliance for speed

---

## Adding New Features

### Adding a New Primitive

**Steps:**

1. **Define structure** in `inc/objects.h`:
```c
typedef struct s_my_primitive {
    t_vec3  param1;
    double  param2;
    t_rgb   color;
} t_my_primitive;
```

2. **Add enum value**:
```c
typedef enum e_object_type {
    // ... existing types
    MY_PRIMITIVE
} t_object_type;
```

3. **Implement intersection test** in `src/logic/hit_my_primitive.c`:
```c
bool hit_my_primitive(t_object *obj, t_ray *ray, t_hit_record *rec) {
    t_my_primitive *prim = (t_my_primitive *)obj->data;

    // Ray-primitive intersection math
    // ...

    if (intersection_found) {
        rec->t = t_value;
        rec->point = intersection_point;
        rec->normal = surface_normal;
        rec->material = &obj->material;
        return true;
    }

    return false;
}
```

4. **Add parser** in `src/parsing/parsing_my_primitive.c`:
```c
bool parse_my_primitive(char **tokens, t_data *data) {
    // Parse parameters
    // Create object
    // Add to list
}
```

5. **Register in main parser** (`src/parsing/parsing.c`):
```c
else if (strcmp(tokens[0], "mp") == 0)  // "mp" = my primitive
    success = parse_my_primitive(tokens, data);
```

6. **Update hit dispatcher** (`src/logic/hittable.c`):
```c
bool hit_object(t_object *obj, t_ray *ray, t_hit_record *rec) {
    if (obj->type == MY_PRIMITIVE)
        return hit_my_primitive(obj, ray, rec);
    // ... other types
}
```

7. **Add bounding box** (`src/logic/bounds.c`):
```c
t_aabb get_object_bounds(t_object *obj) {
    if (obj->type == MY_PRIMITIVE)
        return compute_my_primitive_bounds(obj);
    // ... other types
}
```

8. **Update Makefile**:
```makefile
SRC = ... \
      src/logic/hit_my_primitive.c \
      src/parsing/parsing_my_primitive.c
```

---

### Adding a New Material Type

**Steps:**

1. **Add enum** in `inc/material.h`:
```c
typedef enum e_material_type {
    // ... existing
    MY_MATERIAL
} t_material_type;
```

2. **Implement scatter function** in `src/logic/my_material.c`:
```c
bool my_material_scatter(t_ray *ray, t_hit_record *rec,
                         t_rgb *attenuation, t_ray *scattered) {
    // Material-specific scattering logic
    // ...
    return true;
}
```

3. **Register in ray color** (`src/logic/ray.c`):
```c
else if (rec.material->type == MY_MATERIAL)
    scattered_ok = my_material_scatter(ray, &rec, &attenuation, &scattered);
```

4. **Add parser** (`src/parsing/mat_token.c`):
```c
else if (strcmp(parts[0], "my_material") == 0) {
    material->type = MY_MATERIAL;
    // Parse parameters
}
```

---

## Performance Profiling

### Using gprof

```bash
# Compile with profiling
make CFLAGS="-Wall -Wextra -Ofast -pg"

# Run program
./miniRT scenes/benchmark.rt

# Generate profile
gprof miniRT gmon.out > analysis.txt

# View hotspots
head -50 analysis.txt
```

### Using Valgrind

```bash
# Memory leaks
valgrind --leak-check=full ./miniRT scenes/demo.rt

# Cachegrind (cache profiling)
valgrind --tool=cachegrind ./miniRT scenes/demo.rt
```

---

## Debugging Tips

### Common Issues

**Segfault on render:**
- Check BVH leaf node handling
- Verify all pointers initialized
- Check material pointer validity

**Black screen:**
- Verify camera initialization
- Check ray direction normalization
- Debug with simple scene (single sphere)

**Slow rendering:**
- Enable BVH (`use_bvh = true`)
- Reduce AA samples
- Check for infinite loops in materials

**Network timeout:**
- Check firewall rules
- Verify port not in use
- Use `localhost` for testing

---

This documentation provides a comprehensive guide to the miniRT codebase. For more details on specific functions, refer to the inline comments in the source files.
