#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

class hittable_list : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        double closest_so_far = ray_t.max;

        for (const auto &object : objects)
        {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    int size() const
    {
        return objects.size();
    }

    shared_ptr<hittable> get(int i) const
    {
        return objects[i];
    }

    aabb get_bounding_box()
    {
        aabb bounds;
        for (const auto &object : objects) {
            aabb object_bounds = object->get_bounding_box();
            bounds.growToInclude(object_bounds.min);
            bounds.growToInclude(object_bounds.max);
        }
        return bounds;
    }
};

#endif