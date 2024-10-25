#include "texture.h"
#include "../external/stb_image.h"

image_texture::image_texture(const char *filename)
{
    image = stbi_load(filename, &width, &height, &channels, bytes_per_pixel);
    if (image == nullptr)
    {
        std::cout << "ERROR LOADING IMAGE " << filename << ": " << stbi_failure_reason();
    }
}

color image_texture::get_color_at(const double &u, const double &v) const
{
    interval width_range = interval(0, width);
    interval height_range = interval(0, height);
    int x = u * width;
    int y = v * height;
    width_range.clamp(x);
    height_range.clamp(y);

    unsigned char *c = image;
    c += width * bytes_per_pixel * y + bytes_per_pixel * x;

    int r = c[0];
    int g = c[1];
    int b = c[2];
    // double r = static_cast<double>(ri);
    // double g = static_cast<double>(gi);
    // double b = static_cast<double>(bi);
    // std::cout << r << ", " << g << ", " << b << "\n";
    // std::cout << x << ", " << y << "\n";
    // std::cout << color(r, g, b) * (1.0/255.0) << "\n";
    return color(r, g, b) * (1.0/255.0);
}

image_texture::~image_texture()
{
    STBI_FREE(image);
}