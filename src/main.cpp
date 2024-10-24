#include "rtweekend.h"

#include "camera.h"
#include "sphere.h"
#include "triangle.h"
#include "lambertian_mat.h"
#include "metal_mat.h"
#include "dielectric_mat.h"
#include "bvh_node.h"
#include "model_loader.h"

#include <chrono>

void random_spheres(hittable_list &world)
{
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            // Generate a bunch of random spheres
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 2 * random_double() - .75, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.6)
                {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.75)
                {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.9)
                {
                    sphere_material = make_shared<lambertian>("images/mars.png");
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // Dielectric
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
}

void build_bvh(shared_ptr<bvh_node> root, const hittable_list &world, int max_depth)
{
    for (int i = 0; i < world.size(); i++)
    {
        auto object = world.get(i);
        root->add_object(object);
        root->grow_to_include(object);
    }
    root->split(max_depth);
}

int main()
{
    // World Setup
    hittable_list world;
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    load_file("data/cube.txt", world, ground_material);

    // world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // random_spheres(world);

    auto material1 = make_shared<dielectric>(1.5);
    // world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>("images/earth.png");
    // world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(1.0, 0.6, 0.5), 0.0);
    // world.add(make_shared<sphere>(point3(-1, 0, -8), 0.5, material3));

    // world.add(make_shared<triangle>(point3(-1, 1, -8), point3(1, 1, -8), point3(0, -1, -8), material1));

    // world.add(make_shared<triangle>(point3(-1, -1, -10), point3(1, -1, -10), point3(0, 1, -10), material2));

    // world.add(make_shared<triangle>(point3(-4, -1, -12), point3(4, -1, -12), point3(0, -1, -0), material3));

    // world.add(make_shared<sphere>(point3(3, 0.2, -10), 0.5, ground_material));

    // Build BVH
    std::cout << "Building BVH\n";
    auto bvh_start = std::chrono::high_resolution_clock::now();

    auto root = make_shared<bvh_node>();
    build_bvh(root, world, 5);

    auto bvh_stop = std::chrono::high_resolution_clock::now();
    auto bvh_duration = std::chrono::duration_cast<std::chrono::milliseconds>(bvh_stop - bvh_start);
    std::cout << "BVH Construction complete in " << bvh_duration.count() << " microseconds\n";

    // Camera Setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 4;
    cam.max_depth = 50;

    // Camera Aiming
    cam.vfov = 20;
    cam.lookfrom = point3(5, 5, 5);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // Defocus Blur settings
    cam.defocus_angle = .6;
    cam.focus_dist = 8.5;

    // Render
    std::cout << "Beginning Rendering\n";
    auto render_start = std::chrono::high_resolution_clock::now();

    // for (int i = 1; i <= 10; i++)
    // {
    //     std::cout << "Rendering Image #" << i << "\n";
    //     cam.render(hittable_list(root));
    // }

    cam.render(hittable_list(root));
    // cam.fireSingleRay(hittable_list(root));

    auto render_stop = std::chrono::high_resolution_clock::now();
    auto render_duration = std::chrono::duration_cast<std::chrono::milliseconds>(render_stop - render_start);
    std::cout << "Rendering complete in " << render_duration.count() << " microseconds\nPress enter to exit\n";
    std::cin.get();
}