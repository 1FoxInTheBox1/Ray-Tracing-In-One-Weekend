#include "texture.h"

solid_color_texture::solid_color_texture(const color &albedo) : albedo(albedo) {}
solid_color_texture::solid_color_texture(const double &r, const double &g, const double &b) : albedo(color(r, g, b)) {}

color solid_color_texture::get_color_at(const double &u, const double &v) const {
    return albedo;
}