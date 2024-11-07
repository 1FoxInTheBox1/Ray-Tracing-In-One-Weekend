#include "rtweekend.h"
#include "camera.h"
#include "sphere.h"
#include "mesh.h"
#include "lambertian_mat.h"
#include "metal_mat.h"
#include "dielectric_mat.h"
#include "bvh_node.h"
#include "model_loader.h"
#include "matrix.h"

#include <chrono>

// TODO: I put documentation comments in the .cpp files, 
// find out if they should be moved
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

                if (choose_mat < 0.6)
                {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    mesh_material = make_shared<lambertian>(albedo);
                    if (choose_mesh <= .5)
                    {
                        world.add(make_shared<sphere>(center, 0.2, mesh_material));
                    } 
                    else 
                    {
                        auto cube = make_shared<mesh>("data/cube.txt", center, vec3(0.2, 0.2, 0.2), quaternion(0, 0, 0), mesh_material);
                        cube->add_to_list(world);
                    }
                }
                else if (choose_mat < 0.75)
                {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    mesh_material = make_shared<metal>(albedo, fuzz);
                    if (choose_mesh <= .5)
                    {
                        world.add(make_shared<sphere>(center, 0.2, mesh_material));
                    } 
                    else 
                    {
                        auto cube = make_shared<mesh>("data/cube.txt", center, vec3(0.2, 0.2, 0.2), quaternion(0, 0, 0), mesh_material);
                        cube->add_to_list(world);
                    }
                }
                else if (choose_mat < 0.9)
                {
                    mesh_material = make_shared<lambertian>("images/mars.png");
                    if (choose_mesh <= .5)
                    {
                        world.add(make_shared<sphere>(center, 0.2, mesh_material));
                    } 
                    else 
                    {
                        auto cube = make_shared<mesh>("data/cube.txt", center, vec3(0.2, 0.2, 0.2), quaternion(0, 0, 0), mesh_material);
                        cube->add_to_list(world);
                    }
                }
                else
                {
                    // Dielectric
                    mesh_material = make_shared<dielectric>(1.5);
                    if (choose_mesh <= .5)
                    {
                        world.add(make_shared<sphere>(center, 0.2, mesh_material));
                    } 
                    else 
                    {
                        auto cube = make_shared<mesh>("data/cube.txt", center, vec3(0.2, 0.2, 0.2), quaternion(0, 0, 0), mesh_material);
                        cube->add_to_list(world);
                    }
                }
            }
        }
    }
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

    auto cube1 = make_shared<mesh>("data/teapot.obj", point3(4, .5, 0), vec3(.5, .5, .5), quaternion(0, 0, 0), material1);
    // auto cube1 = make_shared<mesh>("data/cube.txt", point3(0, 1, 0), vec3(1, 1, 1), quaternion(0, 0, 0), material2);
    // auto cube1 = make_shared<mesh>("data/cube.txt", point3(0, 0, 0), vec3(1, 1, 1), quaternion(0, 0, 0), material2);
    // auto cube2 = make_shared<mesh>("data/cube.txt", point3(0, 0, 1.5), vec3(1, 1, 1), quaternion(0, 0, 0, 0), material2);
    auto cube2 = make_shared<mesh>("data/cube.txt", point3(-4, 1, 0), vec3(.5, 2, .5), quaternion(0, 0, 0), material3);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material2));
    // auto cube3 = make_shared<mesh>("data/cube.txt", point3(4, 1, 0), vec3(2, 1, 2), quaternion(0, 0, 0), material1);
    auto cube4 = make_shared<mesh>("data/plane.txt", point3(0, 0, 0), vec3(10, 10, 10), quaternion(0, 0, 0), material4);

    cube1->add_to_list(world);
    cube2->add_to_list(world);
    // cube3->add_to_list(world);
    cube4->add_to_list(world);

    // world.add(make_shared<sphere>(point3(1, 0, 0), .2, material5));
    random_objects(world);

}

int main()
{
    // World Setup
    std::cout << "Building Scene\n";
    auto build_start = std::chrono::high_resolution_clock::now();

    hittable_list world;
    build_scene(world);

    auto build_stop = std::chrono::high_resolution_clock::now();
    auto build_duration = std::chrono::duration_cast<std::chrono::milliseconds>(build_stop - build_start);
    std::cout << "Built scene in " <<build_duration.count() << " milliseconds\n With " << world.size() << " objects\n";

    // Build BVH
    std::cout << "Building BVH\n";
    auto bvh_start = std::chrono::high_resolution_clock::now();

    auto root = make_shared<bvh_node>();
    build_bvh(root, world, 20);

    auto bvh_stop = std::chrono::high_resolution_clock::now();
    auto bvh_duration = std::chrono::duration_cast<std::chrono::milliseconds>(bvh_stop - bvh_start);
    std::cout << "BVH Construction complete in " << bvh_duration.count() << " milliseconds\n";

    // Camera Setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 4;
    cam.max_depth = 50;

    // Camera Aiming
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 6);
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

/*

// Camera Setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    // Camera Aiming
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 6);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // Defocus Blur settings
    cam.defocus_angle = .6;
    cam.focus_dist = 10;

*/