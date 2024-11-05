#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "interval.h"
#include "aabb.h"

#include <vector>

// A representation of a list of hittable objects and useful methods for operating on them
class hittable_list : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list();
    hittable_list(shared_ptr<hittable> object);

    void clear();
    void add(shared_ptr<hittable> object);
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
    int size() const;
    shared_ptr<hittable> get(int i) const;
    aabb get_bounding_box();
};

#endif