#include "utils.h"
#include "hittable.h"
#include "hittablelist.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

int main() {
    //setup camera
    camera sC;
    sC.aspect_ratio = 16.0 / 9.0;
    sC.image_width = 800;
    sC.max_depth = 50;
    sC.samples_per_pixel = 10;

    //world
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.0/1.33);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
    world.add(make_shared<sphere>(point3(+1, 0, -1), 0.5, material_right));
    
    // Render
    sC.render(world);
}