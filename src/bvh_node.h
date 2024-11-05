#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "aabb.h"
#include "hittable_list.h"
#include "interval.h"

// A representation of a node of a BVH tree
class bvh_node : public hittable
{
public:
    shared_ptr<bvh_node> left, right;

    bvh_node();

    void grow_to_include(shared_ptr<hittable> object);
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override;
    void print_bounds();
    hittable_list get_objects() const;
    void add_object(shared_ptr<hittable> object);
    aabb get_bounding_box() override;
    void split(const int &max_depth);

private:
    double midpoint(const double &a, const double &b);
    double get_split_center(const aabb &bounds, int *axis);

private:
    aabb bounds;
    hittable_list objects;
};

#endif