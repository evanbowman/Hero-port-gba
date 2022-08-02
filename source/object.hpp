#pragma once

#include "number/fixnum.hpp"
#include "graphics/sprite.hpp"
#include "platform/platform.hpp"
#include "hitbox.hpp"
#include "list.hpp"
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
        spr_.set_flip({hflip_, false});
        screen.draw(spr_);
    }


    bool dead() const
    {
        return dead_;
    }


    void kill()
    {
        dead_ = true;
    }


    bool place_free(Vec2<Fixnum> pos)
    {
        if (pos.x > 200 or pos.x < 40 or pos.y < 0 or pos.y > 160) {
            return false;
        }
        return true; // TODO...
    }


    const Vec2<Fixnum>& position() const
    {
        return position_;
    }


protected:

    Vec2<Fixnum> position_;

    Hitbox hitbox_;

    int sprite_index_ = 0;
    int sprite_subimage_ = 0;
    bool hflip_ = false;
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



template <typename T, typename... Args>
ObjectRef<T> alloc_object(Args&&... args)
{
    static_assert(sizeof(T) <= 128);
    static_assert(alignof(T) <= alignof(Object));

    if (auto mem = object_pool().alloc()) {
        new (mem) T(std::forward<Args>(args)...);

        return ObjectRef<T>(reinterpret_cast<T*>(mem), object_deleter);
    }

    return {nullptr, object_deleter};
}




}
