#ifndef MATERIAL_H
#define MATERIAL_H

#include "RTWeekend.h"

struct HitRecord;

class Material
{
public:
    virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color& a) : albedo(a) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        auto scatter_direction = record.normal + random_unit_vector();
        (void)(rayIn);

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = record.normal;

        scattered = Ray(record.point, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    Color albedo;
};

class Metal : public Material
{
public:
    Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
    {
        Vector3 reflected = reflect(unit_vector(rayIn.direction()), record.normal);
        scattered = Ray(record.point, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), record.normal) > 0);
    }

public:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = record.front_face ? (1.0 / ir) : ir;

        Vector3 unit_direction = unit_vector(rayIn.direction());
        double cos_theta = fmin(dot(-unit_direction, record.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
            direction = reflect(unit_direction, record.normal);
        else
            direction = refract(unit_direction, record.normal, refraction_ratio);

        scattered = Ray(record.point, direction);
        return true;
    }

public:
    double ir; // Index of Refraction

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif