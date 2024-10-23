#include "hittable_list.h"

hittable_list::hittable_list() {}
hittable_list::hittable_list(shared_ptr<hittable> object) { add(object); }

void hittable_list::clear() { objects.clear(); }

void hittable_list::add(shared_ptr<hittable> object)
{
    objects.push_back(object);
}

bool hittable_list::hit(const ray &r, interval ray_t, hit_record &rec) const
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

int hittable_list::size() const
{
    return objects.size();
}

shared_ptr<hittable> hittable_list::get(int i) const
{
    return objects[i];
}

aabb hittable_list::get_bounding_box()
{
    aabb bounds;
    for (const auto &object : objects) {
        aabb object_bounds = object->get_bounding_box();
        bounds.growToInclude(object_bounds.min);
        bounds.growToInclude(object_bounds.max);
    }
    return bounds;
}