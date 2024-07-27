#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "texture.h"

#include <chrono>

void random_spheres(hittable_list &world) {
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

                if (choose_mat < 0.8)
                {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
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

int main()
{
    // World Setup
    hittable_list world;

    // auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    random_spheres(world);

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    // Camera Setup
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 600;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;

    // Camera Aiming
    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    // Defocus Blur settings
    cam.defocus_angle = .6;
    cam.focus_dist = 10.0;

    // Render
    std::cout << "Beginning Rendering\n";
    auto start = std::chrono::high_resolution_clock::now();
    cam.render(world);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Rendering Complete in " << duration.count() << " microseconds\n Press enter to exit\n";
    std::cin.get();
}