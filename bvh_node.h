#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "aabb.h"
#include "hittable_list.h"

class bvh_node : public hittable
{
public:
    static int count;
    shared_ptr<bvh_node> left, right;
    int id;

    bvh_node() : id(count) {}

    void grow_to_include(shared_ptr<hittable> object)
    {
        aabb object_bounds = object->get_bounding_box();
        bounds.growToInclude(object_bounds.min);
        bounds.growToInclude(object_bounds.max);
    }

    // This function is mostly taken from Ray Tracing: The Next Week
    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        if (!bounds.hit(r, ray_t))
        {
            return false;
        }

        if (left == nullptr)
        {
            if (right == nullptr) {
                return objects.hit(r, ray_t, rec);
            }
            return right->hit(r, ray_t, rec);
        }

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    void print_bounds()
    {
        std::cout << "Lower corner at " << bounds.min << " and greater corner at " << bounds.max << "\n";
    }

    hittable_list get_objects() const
    {
        return objects;
    }

    void add_object(shared_ptr<hittable> object)
    {
        objects.add(object);
    }

    aabb get_bounding_box() override
    {
        return bounds;
    }

private:
    aabb bounds;
    hittable_list objects;
};

int bvh_node::count = 1;

void split_bvh(shared_ptr<bvh_node> parent, const int &max_depth)
{
    // Stop splitting if we reached maximum depth
    if (max_depth == 0)
    {
        return;
    }

    bvh_node::count += 1;
    parent->left = make_shared<bvh_node>();
    bvh_node::count += 1;
    parent->right = make_shared<bvh_node>();

    // Determine center of the parent aabb
    aabb bounds = parent->get_bounding_box();
    double center_x = (bounds.max.x() + bounds.min.x()) / 2.0;

    // Move objects to left or right child depending on if they are
    // on the left or right side of parent aabb
    for (const auto &object : parent->get_objects().objects)
    {
        if (object->get_pos().x() > center_x)
        {
            parent->right->add_object(object);
            parent->right->grow_to_include(object);
        }
        else
        {
            parent->left->add_object(object);
            parent->left->grow_to_include(object);
        }
    }

    split_bvh(parent->right, max_depth - 1);
    split_bvh(parent->left, max_depth - 1);
}

#endif