#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "Transform.h"

class Camera
{
public:
    Transform transform;
    
    Camera() { }

    Ray get_ray(double s, double t)
    {
        Vector3 randomPoint = this->lensRadius * random_in_unit_disk();
        Vector3 offset = this->u * randomPoint.x() + this->v * randomPoint.y();

        Vector3 fromPoint = this->origin + offset;
        Vector3 toDirection = this->lower_left_corner + s * this->horizontal + t * this->vertical - this->origin - offset;
        return Ray(fromPoint, toDirection);
    }
    
    void FOV(double vfov)
    {
        this->fov = vfov;
        this->apply();
    }

    void aspectRatio(double aspectRatio)
    {
        this->camAspectRatio = aspectRatio;
        this->apply();
    }

    void aperture(double aperture)
    {
        this->camAperture = aperture;
        this->apply();
    }

    void focalDistance(double focusDistance)
    {
        this->focusDistance = focusDistance;
        this->apply();
    }

    void position(Vector3 position)
    {
        this->transform.position = position;
        //this->transform.updateTransform();
        this->apply();
    }

    void rotation(Vector3 rotation)
    {
        this->transform.rotation = rotation;
        //this->transform.updateTransform();
        this->apply();
    }

    void lookAt(Vector3 lookAt)
    {
        this->transform.lookAt = lookAt;
        //this->transform.updateTransform();
        this->apply();
    }

private:
    double fov;
    double camAspectRatio;
    
    Vector3 u, v, w;
    double lensRadius;
    double focusDistance;
    double camAperture;

    Point3 origin;
    Point3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;

    void apply()
    {
        auto theta = degrees_to_radians(this->fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = this->camAspectRatio * viewport_height;
        auto worldUp = Vector3(0, 1, 0);

        this->w = unit_vector(this->transform.position - this->transform.lookAt);
        this->u = unit_vector(cross(worldUp, w));
        this->v = cross(w, u);

        this->origin = this->transform.position;
        this->horizontal = this->focusDistance * viewport_width * u;
        this->vertical = this->focusDistance * viewport_height * v;
        this->lower_left_corner = this->origin - (this->horizontal / 2) - (this->vertical / 2) - (this->focusDistance * w);
        this->lensRadius = this->camAperture / 2;
    }
};
#endif