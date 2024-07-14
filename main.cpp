#include "color.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

int main()
{
    int image_width = 256;
    int image_height = 256;
    // Set up output file
    std::ofstream output_file;
    output_file.open("out.ppm");
    output_file << "P3\n"
           << image_width << " " << image_height << "\n255\n";
    
    for (int j = image_height - 1; j >= 0; j--)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            write_color(output_file, pixel_color);
        }
    }
    output_file.close();
}