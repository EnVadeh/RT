#include "utils.h"
#include "hittable.h"
#include "hittablelist.h"
#include "sphere.h"
#include "camera.h"

int main() {
    //setup camera
    camera sC;
    sC.aspect_ratio = 16.0 / 9.0;
    sC.image_width = 800;

    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));
    
    // Render
    sC.render(world);
}