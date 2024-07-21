#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"
#include "rtweekend.h"

#include <iostream>

// TODO: Load image texture
// Use spherical projection to turn hit location
// into u,v coordinate
// Display that color on the sphere

class texture
{
public:
    texture(const char *filename)
    {
        image = stbi_load(filename, &width, &height, &channels, 0);
        if (image == nullptr)
        {
            std::cout << "ERROR LOADING IMAGE " << filename << ": " << stbi_failure_reason();
        }
        else
        {
            std::cout << "Printing image:\n";
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width * channels; j++)
                {
                    unsigned char *c = image + j + (width * i);
                    std::cout << static_cast<void *>(c) << " ";
                    std::cout << static_cast<int>(*c) << " ";
                }
                std::cout << "\n";
            }
        }
    }

    color get_color_at(const double &u, const double &v)
    {
        int x = u * width;
        int y = v * height;
        unsigned char *c = image;
        c += width * y + channels * x;
        
        int r = static_cast<int>(*c);
        int g = static_cast<int>(*(c + 1));
        int b = static_cast<int>(*(c + 2));
        std::cout << r << ", " << g << ", " << b << "\n";
        return color(r, g, b);
    }

    ~texture()
    {
        STBI_FREE(image);
    }

private:
    unsigned char *image;
    int width, height, channels;
};

#endif