#ifndef HITTABLELIST_H
#define HITTABLELIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
    public:
        HittableList() {}
        HittableList(shared_ptr<Hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<Hittable> object) { objects.push_back(object); }

        virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const override;

    public:
        std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const
{
    HitRecord tempRecord;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(ray, t_min, closest_so_far, tempRecord))
        {
            hit_anything = true;
            closest_so_far = tempRecord.t;
            record = tempRecord;
        }
    }

    return hit_anything;
}

#endif