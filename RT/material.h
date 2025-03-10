#pragma once

#include "hittable.h"

//abstract base class
class material {
private:

public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
		return false;
	}
};

class lambertian : public material {
private:
	color albedo;
public:
	lambertian(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
		vec3 scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
private:
	color albedo;
	double fuzz;
public:
	metal(const color& albedo, double fuzz) : albedo(albedo) , fuzz(fuzz) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
};

class dielectric : public material {
private: 
	double refractive_index;
	
	static double reflectance(double cosine, double refraction_index) {
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 *= r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
public:
	dielectric(double refractive_index) : refractive_index(refractive_index) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1, 1, 1);
		double ri = rec.front_face ? (1.0 / refractive_index) : refractive_index;

		vec3 unit_direction = unit_vector(r_in.direction());

		double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = ri * sin_theta > 1.0; //theta ->90 degrees means fresnel reflection
		vec3 direction;

		if (cannot_refract || reflectance(cos_theta, ri) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		scattered = ray(rec.p, direction); //origin will be the hit point
		return true;
	}
};