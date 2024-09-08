#pragma once
#include "hittable.h"


class camera {
private:
	unsigned int image_height;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;

	void initialize() {
		image_height = image_width / aspect_ratio;
		image_height = (image_height < 1) ? 1 : image_height; //atleast 1
	
		center = point3(0, 0, 0); //where camera is

		//determine viewport dimensions
		double focal_length = 1.0;
		double viewport_height = 2.0;
		double viewport_width = viewport_height * (double(image_width) / image_height);

		//vectors across horizontal and down the vertical viewport edges
		vec3 viewport_u = vec3(viewport_width, 0, 0); //towards +ve x axis
		vec3 viewport_v = vec3(0, -viewport_height, 0); //towards -ve y axis
	
		//horizontal and vertical delta vectors from pixel to pixel
		pixel_delta_u = viewport_u / image_width; //offset across x axis
		pixel_delta_v = viewport_v / image_height; //offset across y axis
	
		//location of the upper left pixel (to start from)
		auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v); //first pixel location
	}
	
	color ray_color(const ray& r, const hittable& world) {
		hit_record rec;
		if (world.hit(r, interval(0, infinity), rec)) {
			vec3 direction = random_on_hemisphere(rec.normal); //reflected ray direction
			return 0.5 * ray_color(ray(rec.p, direction), world); //give find color of reflected ray 
		}
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

public:
	unsigned int image_width;
	double aspect_ratio;
	unsigned int max_depth;

	void render(const hittable& world) {
		initialize();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {
				//ray tracing
				auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				auto ray_direction = pixel_center - center;
				ray r(center, ray_direction);

				color pixel_color = ray_color(r, world);
				write_color(std::cout, pixel_color);

			}
		}
		std::clog << "\rDone.                 \n";
	}
};