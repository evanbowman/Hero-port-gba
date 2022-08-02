#include "object.hpp"
#include "engine.hpp"



namespace herocore
{



ObjectPool& object_pool()
{
    static ObjectPool pool("object-pool");
    return pool;
}



GlobalObjectListData::Pool& GlobalObjectListData::pool() const
{
    static Pool pool("object-list-data-pool");
    return pool;
}



// Special copy of hitbox for integer-constrained positions, for faster
// collision checking with walls. Walls don't need Fixed-point positioning
// (yet?)
struct IntHitbox
{
    Vec2<int> position_;

    Hitbox::Dimension dimension_;

    bool overlapping(const IntHitbox& other) const
    {
        const auto c = center();
        const auto oc = other.center();
        if (c.x < (oc.x + other.dimension_.size_.x) and
            (c.x + dimension_.size_.x) > oc.x and
            c.y < (oc.y + other.dimension_.size_.y) and
            (c.y + dimension_.size_.y) > oc.y) {
            return true;
        } else {
            return false;
        }
    }

    Vec2<s16> center() const
    {
        Vec2<s16> c;
        c.x = s16(position_.x) - dimension_.origin_.x;
        c.y = s16(position_.y) - dimension_.origin_.y;
        return c;
    }
};



bool Object::place_free(Vec2<Fixnum> pos)
{
    auto intx = pos.x.as_integer();
    auto inty = pos.y.as_integer();

    // Onscreen area starts at 0, 40.
    intx -= 40;

    const int tile_x = intx / 8;
    const int tile_y = inty / 8;

    Vec2<Fixnum> wall_pos;

    Hitbox wall_hb;
    wall_hb.dimension_.size_ = {8, 8};
    wall_hb.position_ = &wall_pos;

    auto cached_pos = hitbox_.position_;
    hitbox_.position_ = &pos;

    for (int x = -1; x < 2; ++x) {
        for (int y = -1; y < 2; ++y) {
            auto cx = tile_x + x;
            auto cy = tile_y + y;
            if (cx > -1 and cx < 20 and cy > -1 and cy < 20) {
                if (engine().room_.walls_[cx][cy]) {
                    wall_pos.x = 40 + cx * 8;
                    wall_pos.y = cy * 8;

                    if (wall_hb.overlapping(hitbox_)) {
                        hitbox_.position_ = cached_pos;
                        return false;
                    }
                }
            }
        }
    }

    hitbox_.position_ = cached_pos;

    if (pos.x > 200 or pos.x < 40 or pos.y < 0 or pos.y > 160) {
        return false;
    }

    return true; // TODO...
}


}
