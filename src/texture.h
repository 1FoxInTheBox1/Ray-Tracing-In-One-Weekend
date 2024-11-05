#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "rtweekend.h"
#include "color.h"

#include <iostream>

// An abstract class representing a texture
class texture
{
public:
    virtual color get_color_at(const double &u, const double &v) const = 0;
};

// A class representing a texture consisting only of one color
class solid_color_texture : public texture
{
public:
    solid_color_texture(const color &albedo);
    solid_color_texture(const double &r, const double &g, const double &b);

    color get_color_at(const double &u, const double &v) const override;

private:
    color albedo;
};

// A class representing a texture that pulls colors from an image
class image_texture : public texture
{
public:
    image_texture(const char *filename);

    color get_color_at(const double &u, const double &v) const override;

    ~image_texture();

private:
    unsigned char *image;
    int bytes_per_pixel = 3;
    int width, height, channels;
};

#endif