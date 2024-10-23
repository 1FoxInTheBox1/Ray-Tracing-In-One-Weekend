#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "rtweekend.h"
#include "color.h"

#include <iostream>

class texture
{
public:
    virtual color get_color_at(const double &u, const double &v) const = 0;
};

class solid_color_texture : public texture
{
public:
    solid_color_texture(const color &albedo);
    solid_color_texture(const double &r, const double &g, const double &b);

    color get_color_at(const double &u, const double &v) const override;

private:
    color albedo;
};

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