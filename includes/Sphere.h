#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vector3.h"

class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 cen, double r, shared_ptr<Material> mat) : center(cen), radius(r), mat_ptr(mat) {};

        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;

    public:
        Point3 center;
        double radius;
        shared_ptr<Material> mat_ptr;
};

bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
{
    Vector3 oc = ray.origin() - center;
    auto a = ray.direction().length_squared();
    auto half_b = dot(oc, ray.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
        return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    record.t = root;
    record.point = ray.at(record.t);
    Vector3 outward_normal = (record.point - center) / radius;
    record.set_face_normal(ray, outward_normal);
    record.mat_ptr = mat_ptr;

    return true;
}

#endif