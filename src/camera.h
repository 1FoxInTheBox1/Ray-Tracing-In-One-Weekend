#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"

#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

// A representation of the scene's camera. Also contains rendering logic.
class camera
{
public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int image_width = 100;      // Rendered image width in pixel count
    int samples_per_pixel = 10; // Count of random samples for each pixel
    int max_depth = 10;         // Maximum number of ray bounces
    int lines_remaining = 0;    // Number of horizontal lines that must stil be rendered
    std::mutex cam_mtx;         // Mutex for synchronizing threads

    double vfov = 90;                  // Vertical view angle (field of view)
    point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
    point3 lookat = point3(0, 0, -1);  // Point camera is looking at
    vec3 vup = vec3(0, 1, 0);          // Camera-relative "up" direction

    double defocus_angle = 0; // Variation angle of rays through each pixel
    double focus_dist = 10;   // Distance from camera lookfrom to focus plane

    void render(const hittable &world);
    // Fires a single ray at the center of the screen.
    // Used for testing
    void fire_single_ray(const hittable &world);

private:
    void initialize();
    void run_thread(const hittable &world, const int thread_num, std::vector<color> &completed);
    ray get_ray(int i, int j) const;
    vec3 sample_square() const;
    vec3 defocus_disk_sample() const;
    color ray_color(const ray &r, int depth, const hittable &world) const;

private:
    int image_height;           // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    point3 center;              // Camera center
    point3 pixel00_loc;         // Location of pixel 0, 0
    vec3 pixel_delta_u;         // Offset to pixel to the right
    vec3 pixel_delta_v;         // Offset to pixel below
    vec3 u, v, w;               // Camera frame basis vectors
    vec3 defocus_disk_u;        // Defocus disk horizontal radius
    vec3 defocus_disk_v;        // Defocus disk vertical radius
};

#endif