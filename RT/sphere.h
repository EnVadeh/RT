#pragma once
#include "hittable.h"

class sphere : public hittable {
private:
    point3 center;
    double radius;
    shared_ptr <material> mat;
public:
    sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = center - r.origin(); //direction from center origin of ray to the center of the sphere
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = h * h - a * c;

        //return discriminant >= 0; //0 means 1 root, +ve value only means more tha 1 root
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        //nearest root that lies in the acceptable range of the ray 
        auto root = (h - sqrtd) / a; // - of the plusorminus
        if(!ray_t.sorrounds(root)){ //if it's not between min and max
            root = (h + sqrtd) / a; // + of the plusorminus
            if (!ray_t.sorrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t); //vetor for where it hit
        vec3 outward_normal = (rec.p - center) / radius; //basically normalising it easily
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;
        return true;
    }
};

