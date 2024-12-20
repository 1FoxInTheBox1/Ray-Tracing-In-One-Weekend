#include "rtweekend.h"
#include "camera.h"
#include "sphere.h"
#include "mesh.h"
#include "lambertian_mat.h"
#include "metal_mat.h"
#include "dielectric_mat.h"
#include "emissive_mat.h"
#include "universal_mat.h"
#include "bvh_node.h"
#include "model_loader.h"
#include "matrix.h"
#include "mesh_instance.h"

#include <chrono>

// TODO: I put documentation comments in the .cpp files,
// find out if they should be moved

// TODO: Find a good spot for these
auto pot = make_shared<mesh>("data/teapot.obj");
auto cube = make_shared<mesh>("data/cube.txt");
auto plane = make_shared<mesh>("data/plane.txt");
auto square = make_shared<mesh>("data/square.txt");
auto mesh_sphere = make_shared<mesh>("data/sphere.txt");

void add_test_mesh(double choose_mesh, point3 center, shared_ptr<material> mesh_material, hittable_list &world)
{
    if (choose_mesh <= .45)
    {
        world.add(make_shared<sphere>(center, 0.2, mesh_material));
    }
    else if (choose_mesh <= .9)
    {
        auto cube_instance = make_shared<mesh_instance>(cube, center, vec3(0.2, 0.2, 0.2), quaternion(0, 0, 0), mesh_material);
        cube_instance->add_to_list(world);
    }
    else
    {
        auto pot_instance = make_shared<mesh_instance>(pot, center, vec3(0.1, 0.1, 0.1), quaternion(0, 0, 0), mesh_material);
        pot_instance->add_to_list(world);
    }
}

void random_objects(hittable_list &world)
{
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            // Generate a bunch of random spheres
            auto choose_mat = random_double();
            auto choose_mesh = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> mesh_material;

                if (choose_mat < 0.5)
                {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    mesh_material = make_shared<lambertian>(albedo);
                    add_test_mesh(choose_mesh, center, mesh_material, world);
                }
                else if (choose_mat < 0.75)
                {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    mesh_material = make_shared<metal>(albedo, fuzz);
                    add_test_mesh(choose_mesh, center, mesh_material, world);
                }
                else if (choose_mat < 0.9)
                {
                    // Albedo texture
                    mesh_material = make_shared<lambertian>("images/mars.png");
                    add_test_mesh(choose_mesh, center, mesh_material, world);
                }
                else
                {
                    // Dielectric
                    mesh_material = make_shared<dielectric>(1.5);
                    add_test_mesh(choose_mesh, center, mesh_material, world);
                }
            }
        }
    }
}

void add_box(hittable_list &world)
{
    auto wall_mat = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto mirror_mat = make_shared<metal>(color(1.0, 1.0, 1.0), 0.01);
    auto light_mat = make_shared<emissive>(color(3.0, 3.0, 3.0));

    world.add(make_shared<triangle>(point3(2, -2, 2), point3(2, 2, 2), point3(2, 2, -2), wall_mat));
    world.add(make_shared<triangle>(point3(2, -2, 2), point3(2, 2, -2), point3(2, -2, -2), wall_mat));

    world.add(make_shared<triangle>(point3(-2, -2, 2), point3(-2, 2, 2), point3(-2, 2, -2), wall_mat));
    world.add(make_shared<triangle>(point3(-2, -2, 2), point3(-2, 2, -2), point3(-2, -2, -2), wall_mat));

    world.add(make_shared<triangle>(point3(2, -2, 2), point3(2, 2, 2), point3(-2, 2, 2), mirror_mat));
    world.add(make_shared<triangle>(point3(2, -2, 2), point3(-2, 2, 2), point3(-2, -2, 2), mirror_mat));

    world.add(make_shared<triangle>(point3(2, -2, -2), point3(2, 2, -2), point3(-2, 2, -2), mirror_mat));
    world.add(make_shared<triangle>(point3(2, -2, -2), point3(-2, 2, -2), point3(-2, -2, -2), mirror_mat));

    world.add(make_shared<triangle>(point3(2, -2, 2), point3(-2, -2, 2), point3(-2, -2, -2), wall_mat));
    world.add(make_shared<triangle>(point3(2, -2, 2), point3(-2, -2, -2), point3(2, -2, -2), wall_mat));

    world.add(make_shared<triangle>(point3(2, 2, 2), point3(-2, 2, 2), point3(-2, 2, -2), wall_mat));
    world.add(make_shared<triangle>(point3(2, 2, 2), point3(-2, 2, -2), point3(2, 2, -2), wall_mat));

    world.add(make_shared<triangle>(point3(.3, .299, .3), point3(-.3, .299, .3), point3(-.3, .299, -.3), light_mat));
    world.add(make_shared<triangle>(point3(.3, .299, .3), point3(-.3, .299, -.3), point3(.3, .299, -.3), light_mat));
}

void build_bvh(shared_ptr<bvh_node> &root, const hittable_list &world, int max_depth)
{
    for (int i = 0; i < world.size(); i++)
    {
        auto object = world.get(i);
        root->add_object(object);
        root->grow_to_include(object);
    }
    root->split(max_depth);
}

void build_scene(hittable_list &world)
{
    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>("images/earth.png");
    auto material3 = make_shared<metal>(color(1.0, 0.6, 0.5), 0.0);
    auto material4 = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material5 = make_shared<lambertian>(color(1.0, 0.0, 1.0));
    auto material6 = make_shared<emissive>(color(4.0, 0.0, 0.0));
    auto material7 = make_shared<metal>(color(0.6, 0.6, 0.6), 1.0);
    auto material8 = make_shared<emissive>(color(100.0, 100.0, 100.0));
    auto material9 = make_shared<lambertian>(color(0.0, 1.0, 1.0));
    auto material10 = make_shared<lambertian>("images/circuit/circuitry_albedo.png");
    auto pbr_mat = make_shared<universal>("images/circuit/circuitry_albedo.png", "images/circuit/circuitry_normals.png",
                                          "images/circuit/circuitry_metallic.png", "images/circuit/circuitry_emission.png", 10);

    // auto pot_instance = make_shared<mesh_instance>(pot, point3(4, .5, 0), vec3(.5, .5, .5), quaternion(0, 0, 0), material1);
    // auto cube_instance = make_shared<mesh_instance>(cube, point3(-4, 1, 0), vec3(.5, 2, .5), quaternion(0, 0, 0), material3);
    // auto plane_instance = make_shared<mesh_instance>(plane, point3(0, 0, 0), vec3(10, 10, 10), quaternion(0, 0, 0), material4);
    // world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material2));

    // pot_instance->add_to_list(world);
    // cube_instance->add_to_list(world);
    // plane_instance->add_to_list(world);


    auto plane_instance = make_shared<mesh_instance>(square, point3(-5, -0.5, -5), vec3(10, 10, 10), quaternion(pi/2, 0, 0), material4);
    // auto light1 = make_shared<mesh_instance>(square, point3(-1, 0, -2), vec3(2.0, 1.0, 1.0), quaternion(0, 0, 0), material8);
    // auto light2 = make_shared<mesh_instance>(square, point3(-1, 0, 2), vec3(2.0, 1.0, 1.0), quaternion(0, 0, 0), material8);

    world.add(make_shared<sphere>(point3(.55, 0, 0), 0.5, pbr_mat));
    world.add(make_shared<sphere>(point3(-.55, 0, 0), 0.5, material4));
    // world.add(make_shared<sphere>(point3(-.55, 0, 0), 0.5, material7));
    // world.add(make_shared<sphere>(point3(-.55, 0, 0), 0.5, material7));

    // auto cube_instance = make_shared<mesh_instance>(cube, point3(-.55, 0, 0), vec3(.5, .5, .5), quaternion(0, 0, 0), pbr_mat);
    // cube_instance->add_to_list(world);
    // auto sphere_instance = make_shared<mesh_instance>(mesh_sphere, point3(-.55, 0, 0), vec3(.5, .5, .5), quaternion(0, 0, 0), pbr_mat);
    // sphere_instance->add_to_list(world);

    plane_instance->add_to_list(world);
    // light1->add_to_list(world);
    // light2->add_to_list(world);
}

// random_objects(world);

int main()
{
    // World Setup
    std::cout << "Building Scene\n";
    auto build_start = std::chrono::high_resolution_clock::now();

    hittable_list world;
    build_scene(world);

    auto build_stop = std::chrono::high_resolution_clock::now();
    auto build_duration = std::chrono::duration_cast<std::chrono::milliseconds>(build_stop - build_start);
    std::cout << "Built scene in " << build_duration.count() << " milliseconds\n With " << world.size() << " objects\n";

    // Build BVH
    std::cout << "Building BVH\n";
    auto bvh_start = std::chrono::high_resolution_clock::now();

    auto root = make_shared<bvh_node>();
    build_bvh(root, world, 14);

    auto bvh_stop = std::chrono::high_resolution_clock::now();
    auto bvh_duration = std::chrono::duration_cast<std::chrono::milliseconds>(bvh_stop - bvh_start);
    std::cout << "BVH Construction complete in " << bvh_duration.count() << " milliseconds\n";

    // Camera Setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    // Camera Aiming
    cam.vfov = 40;
    cam.lookfrom = point3(0, 0, -2.5);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // Defocus Blur settings
    cam.defocus_angle = 0;
    cam.focus_dist = 10;

    // Render
    std::cout << "Beginning Rendering\n";
    auto render_start = std::chrono::high_resolution_clock::now();

    cam.render(hittable_list(root));
    // cam.fire_single_ray(hittable_list(root));

    auto render_stop = std::chrono::high_resolution_clock::now();
    auto render_duration = std::chrono::duration_cast<std::chrono::milliseconds>(render_stop - render_start);
    std::cout << "Rendering complete in " << render_duration.count() << " milliseconds\nPress enter to exit\n";

    std::cin.get();
}