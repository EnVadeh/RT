#pragma once
#include "vec3.h"

class ray {
private:
	vec3 dir;
	point3 orig; //same thing as vec3 but just nomenclature difference
public:
	ray() {}
	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
	
	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }
	
	point3 at(double t) const { //idk why I thought at was a keyword and not a funciotn I defined
		return orig + dir * t;
	}
};