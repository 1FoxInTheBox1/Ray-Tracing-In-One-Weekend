#include "bvh_node.h"

bvh_node::bvh_node() {}

// Expands the node to cover the object given as a parameter
void bvh_node::grow_to_include(shared_ptr<hittable> object)
{
    aabb object_bounds = object->get_bounding_box();
    bounds.growToInclude(object_bounds.min);
    bounds.growToInclude(object_bounds.max);
}

bool bvh_node::hit(const ray &r, interval ray_t, hit_record &rec) const
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

void bvh_node::print_bounds()
{
    std::cout << "Lower corner at " << bounds.min << " and greater corner at " << bounds.max << "\n";
}

hittable_list bvh_node::get_objects() const
{
    return objects;
}

void bvh_node::add_object(shared_ptr<hittable> object)
{
    objects.add(object);
}

aabb bvh_node::get_bounding_box()
{
    return bounds;
}

// Splits the node into a left and right subnode
// along the node's largest side
void bvh_node::split(const int &max_depth)
{
    // Stop splitting if we reached maximum depth
    if (max_depth == 0 || get_objects().size() <= 1)
    {
        return;
    }

    left = make_shared<bvh_node>();
    right = make_shared<bvh_node>();

    // Determine center of the parent aabb
    int axis = get_split_axis();
    double center = 0;

    // Get average position of objects
    for (const auto &object : get_objects().objects)
    {
        double object_pos = object->get_pos()[axis];
        center += object_pos;
    }
    center /= get_objects().size();

    // Move objects to left or right child depending on which
    // side of the AABB they are on
    for (const auto &object : get_objects().objects)
    {
        double object_pos = object->get_pos()[axis];
        if (axis == -1)
        {
            std::cout << "An error occured while building the BVH\n";
            return;
        }

        if (object_pos > center)
        {
            right->add_object(object);
            right->grow_to_include(object);
        }
        else
        {
            left->add_object(object);
            left->grow_to_include(object);
        }
    }

    right->split(max_depth - 1);
    left->split(max_depth - 1);
}

double bvh_node::midpoint(const double &a, const double &b)
{
    return (a + b) / 2.0;
}

int bvh_node::get_split_axis()
{
    double x_length = bounds.max.x() - bounds.min.x();
    double y_length = bounds.max.y() - bounds.min.y();
    double z_length = bounds.max.z() - bounds.min.z();

    if (x_length > y_length)
    {
        if (x_length > z_length)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        if (y_length > z_length)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}