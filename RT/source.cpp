#include "utils.h"
#include "hittable.h"
#include "hittablelist.h"
#include "sphere.h"



color ray_color(const ray& r, const hittable& world) { 
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
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
     
    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            //ray tracing
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);

        }
    }
    std::clog << "\rDone.                 \n";

}