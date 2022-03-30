
cmake_minimum_required(VERSION 3.15)

project(raytracer)

add_executable(raytracer 
  src/main.c 
  src/utility.c

  src/image/bitmap.c 

  src/materials/diffuse.c 
  src/materials/emissive.c 
  src/materials/mat_list.c 
  src/materials/metal.c

  src/objects/mesh.c 
  src/objects/plane.c 
  src/objects/sphere.c 

  src/renderer/renderer_compute.c 
  src/renderer/renderer_misc.c
  src/renderer/renderer_trace.c 

  src/structs/vector.c
  src/structs/hit_record.c 
  src/structs/ray.c 
)

target_link_libraries(raytracer m)

