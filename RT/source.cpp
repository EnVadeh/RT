#include "color.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>

float aspect_ratio = 16.0 / 9.0;
int image_width = 400;

int main() {
    // Image
    int image_height = image_width/aspect_ratio;
    image_height = (image_height < 1) ? 1 : image_height; //atleast 1

    float viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    // Ray
    vec3 direction(0, 0, -1);
    point3 origin(0, 0, 0);
    ray Ray1(origin, direction);
    point3 Rayx = Ray1.at(3);
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        //std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        std::clog << "The ray is at: " << Rayx<< std::endl<< std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            write_color(std::cout, pixel_color);

        }
    }
    std::clog << "\rDone.                 \n";

}