#include "camera.h"

void camera::render(const hittable &world)
{
    initialize();

    // Set up output file
    std::ofstream output_file;
    output_file.open("out.ppm");
    output_file << "P3\n"
                << image_width << " " << image_height << "\n255\n";

    // Create threads
    std::thread threads[NUM_THREADS];
    std::vector<color> completed[NUM_THREADS];
    lines_remaining = image_height;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        completed[i] = std::vector<color>();
        threads[i] = std::thread(&camera::run_thread, this, std::ref(world), i, std::ref(completed[i]));
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }

    // Write colors
    for (int i = 0; i < NUM_THREADS; i++)
    {
        for (auto pixel : completed[i])
        {
            write_color(output_file, pixel);
        }
    }

    std::clog << "\rDone.                 \n";
    output_file.close();
}

// Runs a single thread.
// These threads write image data to the completed vector, which is later copied to the final image
void camera::run_thread(const hittable &world, const int thread_num, std::vector<color> &completed)
{
    int work_size = image_height / NUM_THREADS;
    int work_start = work_size;
    work_size += (thread_num % NUM_THREADS == NUM_THREADS - 1) ? image_height % work_size : 0;
    // Render
    for (int j = 0; j < work_size; j++)
    {
        cam_mtx.lock();
        lines_remaining--;
        std::clog << "\rScanlines remaining: " << lines_remaining << ' ' << std::flush;
        cam_mtx.unlock();
        // auto render_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < image_width; i++)
        {
            color pixel_color(0, 0, 0);
            // Get random samples around each pixel and then
            // average the results to determine the final displayed color
            for (int sample = 0; sample < samples_per_pixel; sample++)
            {
                ray r = get_ray(i, j + (work_start * thread_num));
                pixel_color += ray_color(r, max_depth, world);
            }
            completed.push_back(pixel_samples_scale * pixel_color);
        }
        // auto render_stop = std::chrono::high_resolution_clock::now();
        // auto render_duration = std::chrono::duration_cast<std::chrono::milliseconds>(render_stop - render_start);
        // std::cout << "Scanline " << j << " took " << render_duration.count() << " milliseconds\n";
    }
}

// Shoots a single ray at the center of the screen
// Just used for debugging
void camera::fire_single_ray(const hittable &world)
{
    initialize();
    ray r = get_ray(image_width / 2, image_height / 2);
    color c = ray_color(r, max_depth, world);
    std::cout << "Got color: " << c << "\n";
}

void camera::initialize()
{
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;

    // Viewport Dimensions
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2 * h * focus_dist;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Calculate unit basis vectors
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate Viewport Vectors
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // Calculate horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors
    auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

// Construct a camera ray originating from the defocus disk and directed at randomly sampled
// point around the pixel location i, j
ray camera::get_ray(int i, int j) const
{
    // Get the randomly sampled point
    // We'll eventually call get_ray a bunch and average all those results
    // We do this to implement antialiasing
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

    // Get the random origin point
    // We do this to simulate a camera lens, which is not a single point but a disk.
    // The result of this approximation is defocus blur
    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

// Returns the vector to a random point in the [-.5, -.5]-[+.5,+.5] unit square
vec3 camera::sample_square() const
{
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

// Returns a random point in the camera defocus disk
vec3 camera::defocus_disk_sample() const
{
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

// Determines the final color of a ray
// A ray that reaches the depth limit return black
color camera::ray_color(const ray &r, int depth, const hittable &world) const
{
    // If we've exceeded the ray bounce limit, return black
    if (depth <= 0)
    {
        return color(0, 0, 0);
    }

    hit_record rec;

    if (world.hit(r, interval(0.001, infinity), rec))
    {
        ray scattered;
        ray emitted;
        color attenuation;

        // If scatter() returns true then the ray was not absorbed
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            // We recursively determine the color of the scattered ray to determine the color of this ray
            return attenuation * ray_color(scattered, depth - 1, world);
        if (rec.mat->emit(r, rec, attenuation, emitted))
            return attenuation;
        // An absorbed ray obviously won't produce any color, so we return black
        return color(0, 0, 0);
    }

    // Create background gradient with a linear interpolation
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}