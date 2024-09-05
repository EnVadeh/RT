#pragma once
#include "utils.h"

class hit_record {
public:
	point3 p; //where it's hitting
	vec3 normal; //normal where it's hit
	double t;
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0; //comign from outside, cos angle more than 180
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};

