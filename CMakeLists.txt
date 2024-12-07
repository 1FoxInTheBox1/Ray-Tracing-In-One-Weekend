cmake_minimum_required(VERSION 3.14)

project(RayTracing VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/rtweekend.h
    src/aabb.h
    src/aabb.cpp
    src/bvh_node.cpp
    src/bvh_node.h
    src/camera.h
    src/camera.cpp
    src/color.h
    src/color.cpp
    src/dielectric_mat.h
    src/dielectric_mat.cpp
    src/lambertian_mat.h
    src/lambertian_mat.cpp
    src/metal_mat.h
    src/metal_mat.cpp
    src/emissive_mat.h
    src/emissive_mat.cpp
    src/universal_mat.h
    src/universal_mat.cpp
    src/hittable_list.h
    src/hittable_list.cpp
    src/hittable.h
    src/hittable.cpp
    src/image_texture.cpp
    src/interval.h
    src/interval.cpp
    src/material.h
    src/ray.h
    src/ray.cpp
    src/solid_color_texture.cpp
    src/sphere.h
    src/sphere.cpp
    src/texture.h
    src/triangle.h
    src/triangle.cpp
    src/vec3.h
    src/vec3.cpp
    src/quaternion.h
    src/quaternion.cpp
    src/matrix.h
    src/matrix.cpp
    src/model_loader.h
    src/model_loader.cpp
    src/mesh.h
    src/mesh.cpp
    src/mesh_instance.h
    src/mesh_instance.cpp
    external/stb_image.h
    )