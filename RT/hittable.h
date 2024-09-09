#pragma once
#include "utils.h"

class material;

class hit_record { //for a single ray, last time it hit and attributes related to it's hit
public:
	point3 p; //where it's hitting
	vec3 normal; //normal where it's hit
	shared_ptr<material> mat;
	double t; //how far from the ray's origin the intersection/hit takes place
	bool front_face;

	void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0; //comign from outside, cos angle more than 180
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

