#pragma once
#include "hittable.h"


class camera {
private:
	unsigned int image_height;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	double pixel_samples_scale; //color scale factor for a sum of pixel samples

	void initialize() {
		image_height = image_width / aspect_ratio;
		image_height = (image_height < 1) ? 1 : image_height; //atleast 1
	
		pixel_samples_scale = 1.0 / samples_per_pixel;
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
	vec3 sample_square() const {
		// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	ray get_ray(size_t i, size_t j) const {
	    // Construct a camera ray originating from the origin and directed at randomly sampled
	    // point around the pixel location i, j.
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
			+ ((i + offset.x()) * pixel_delta_u)
			+ ((j + offset.y()) * pixel_delta_v);
		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	color ray_color(const ray& r, unsigned int depth, const hittable& world) {
		if (depth == 0)
			return color(0, 0, 0);
		hit_record rec;
		if (world.hit(r, interval(0.001, infinity), rec)) {
			vec3 direction = random_on_hemisphere(rec.normal); //reflected ray direction
			return 0.5 * ray_color(ray(rec.p, direction), depth-1,world); //give find color of reflected ray 
		}
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

public:
	unsigned int image_width;
	double aspect_ratio;
	unsigned int max_depth;
	unsigned int samples_per_pixel;

	void render(const hittable& world) {
		initialize();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		for (size_t j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (size_t i = 0; i < image_width; i++) {
				//ray tracing
				color pixel_color(0, 0, 0);
				for (size_t sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);//very inffecient i think
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_samples_scale * pixel_color);

			}
		}
		std::clog << "\rDone.                 \n";
	}
};