#pragma once

#include "graphics/sprite.hpp"
#include "hitbox.hpp"
#include "list.hpp"
#include "number/fixnum.hpp"
#include "platform/platform.hpp"
#include <memory>


namespace herocore
{


class Object
{
public:
    Object() : hitbox_{&position_, {0, 0, 0, 0}}
    {
    }


    virtual ~Object()
    {
    }


    virtual void step()
    {
        position_ = position_ + speed_;
    }


    virtual void draw(Platform::Screen& screen) const
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_);
        spr_.set_position(position_);
        spr_.set_flip({hflip_, vflip_});
        screen.draw(spr_);
    }


    bool dead() const
    {
        return dead_;
    }


    virtual void kill()
    {
        dead_ = true;
    }


    bool place_free(Vec2<Fixnum> pos);


    bool place_free(Vec2<Fixnum> pos, int diameter);


    Fixnum& y()
    {
        return position_.y;
    }


    Fixnum& x()
    {
        return position_.x;
    }


    const Fixnum& y() const
    {
        return position_.y;
    }


    const Fixnum& x() const
    {
        return position_.x;
    }


    const Vec2<Fixnum>& position() const
    {
        return position_;
    }


    void set_position(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
    }


    const Hitbox& hitbox() const
    {
        return hitbox_;
    }


    const auto& speed() const
    {
        return speed_;
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


    Vec2<bool> get_flip() const
    {
        return {hflip_, vflip_};
    }


    void set_flip(bool hflip, bool vflip)
    {
        hflip_ = hflip;
        vflip_ = vflip;
    }


protected:
    Vec2<Fixnum> position_;

    Hitbox hitbox_;

    int sprite_index_ = 0;
    int sprite_subimage_ = 0;
    bool hflip_ = false;
    bool vflip_ = false;
    Vec2<u8> origin_;

    Vec2<Fixnum> speed_;

    bool dead_ = false;
};


static constexpr const int object_pool_size = 128;


template <typename T> using ObjectRef = std::unique_ptr<T, void (*)(Object*)>;


using ObjectNode = BinaryNode<ObjectRef<Object>>;


struct GlobalObjectListData
{
    using Pool = ::Pool<sizeof(ObjectNode), object_pool_size, alignof(Object)>;

    Pool& pool() const;
};


template <typename T> struct GlobalObjectListDataImpl : GlobalObjectListData
{
    BinaryNode<T>* begin_;
};


template <u32 Capacity>
using ObjectNodePool = Pool<sizeof(ObjectNode), Capacity, alignof(Object)>;


template <typename T>
using EntityListData = ListData<T, ObjectNodePool<object_pool_size>>;


template <typename T>
using ObjectList = List<ObjectRef<T>, GlobalObjectListDataImpl<ObjectRef<T>>>;


using ObjectPool = Pool<128, object_pool_size, alignof(Object)>;
ObjectPool& object_pool();


inline void object_deleter(Object* obj)
{
    if (obj) {
        obj->~Object();
        object_pool().free(reinterpret_cast<u8*>(obj));
    }
}


inline ObjectRef<Object> null_object()
{
    return {nullptr, object_deleter};
}


template <typename T, typename... Args> ObjectRef<T> alloc_object(Args&&... args)
{
    static_assert(sizeof(T) <= 128);
    static_assert(alignof(T) <= alignof(Object));

    if (auto mem = object_pool().alloc()) {
        new (mem) T(std::forward<Args>(args)...);

        return ObjectRef<T>(reinterpret_cast<T*>(mem), object_deleter);
    }

    return {nullptr, object_deleter};
}


} // namespace herocore
