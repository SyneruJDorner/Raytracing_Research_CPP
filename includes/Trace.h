#ifndef TRACE_HPP
# define TRACE_HPP

#include "Hittable.h"
#include "Material.h"

static Color trace(const Ray& ray, const Hittable& world, int depth)
{
    HitRecord record;
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color(0, 0, 0);

    if (world.hit(ray, 0.001, infinity, record))
    {
        Ray scattered;
        Color attenuation;
        if (record.mat_ptr->scatter(ray, record, attenuation, scattered))
            return attenuation * trace(scattered, world, depth-1);
        return Color(0, 0, 0);
    }

    Vector3 unit_direction = unit_vector(ray.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

#endif