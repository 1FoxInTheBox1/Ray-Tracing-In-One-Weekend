#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "aabb.h"
#include "hittable_list.h"

class bvh_node : public hittable
{
public:
    shared_ptr<bvh_node> left, right;

    bvh_node() {}

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
            if (right == nullptr)
            {
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

double midpoint(const double &a, const double &b)
{
    return (a + b) / 2.0;
}

double get_split_center(const aabb &bounds, int *axis)
{
    double x_length = bounds.max.x() - bounds.min.x();
    double y_length = bounds.max.y() - bounds.min.y();
    double z_length = bounds.max.z() - bounds.min.z();

    if (x_length > y_length)
    {
        if (x_length > z_length)
        {
            *axis = 0;
            return midpoint(bounds.max.x(), bounds.min.x());
        }
        else
        {
            *axis = 2;
            return midpoint(bounds.max.z(), bounds.min.z());
        }
    }
    else
    {
        if (y_length > z_length)
        {
            *axis = 1;
            return midpoint(bounds.max.y(), bounds.min.y());
        }
        else
        {
            *axis = 2;
            return midpoint(bounds.max.z(), bounds.min.z());
        }
    }
}

void split_bvh(shared_ptr<bvh_node> parent, const int &max_depth)
{
    // Stop splitting if we reached maximum depth
    if (max_depth == 0 || parent->get_objects().size() <= 1)
    {
        return;
    }

    parent->left = make_shared<bvh_node>();
    parent->right = make_shared<bvh_node>();

    // Determine center of the parent aabb
    aabb bounds = parent->get_bounding_box();
    int axis = -1;
    double center = get_split_center(bounds, &axis);

    // Move objects to left or right child depending on which
    // side of the AABB they are on
    for (const auto &object : parent->get_objects().objects)
    {
        double object_pos = object->get_pos().x();
        if (axis == 2)
        {
            object_pos = object->get_pos().z();
        }
        if (axis == 1)
        {
            object_pos = object->get_pos().y();
        }
        if (axis == -1)
        {
            std::cout << "An error occured while building the BVH";
            return;
        }

        if (object_pos > center)
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