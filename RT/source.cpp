#include "color.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin(); //direction from center origin of ray to the center of the sphere
    auto a = dot(r.direction(), r.direction());
    auto b = -2 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    //return discriminant >= 0; //0 means 1 root, +ve value only means more tha 1 root
    if (discriminant < 0)
        return -1.0;
    else
        return (( - b - sqrt(discriminant)) / (2.0 * a)); //at what distance from the ray origin is it intersected
}


color ray_color(const ray& r) { 

    auto t = hit_sphere(point3(0, 0, -1), 0.5, r); //-1 is exactly at the viewport so it's size isnt' distoreted
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));//unit vector from center to where the intersection happens
        return 0.5 * color(N[0], N[1], N[2]);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    float aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = image_width/aspect_ratio;
    image_height = (image_height < 1) ? 1 : image_height; //atleast 1

    //camera
    float focal_length = 1.0; //how far is the camera from the viewpoert
    float viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    vec3 camera_center = point3(0, 0, 0);

    //vectors across horizontal and down the vertical viewport edges
    vec3 viewport_u = vec3(viewport_width, 0, 0); //towards +ve x axis
    vec3 viewport_v = vec3(0, -viewport_height, 0); //towards -ve y axis, basically down
    
    //horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width; //offset across x axis
    auto pixel_delta_v = viewport_v / image_height; //offset across y axis

    //location of the upper left pixel (to start from)
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); //first pixel location
     
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            //ray tracing
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);

        }
    }
    std::clog << "\rDone.                 \n";

}