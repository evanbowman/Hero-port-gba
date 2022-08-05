#pragma once

#include "object.hpp"



// Used to associate groups of objects with integer tags. The original game
// often checks overlap between objects of the same type, to prevent them from
// bunching up, we need a relatively fast way to check intersection between
// objects of the exact same type, i.e. we need to group objects in some
// way. Alternatively, I guess we could just check collisions between all
// objects and then check the type upon collision, or plenty of other
// methods. But this is what I decided to do.



namespace herocore
{



class TaggedObject : public Object
{
public:

    enum class Tag : u8 {
        drone,
        reaver,
        crusher,
        spew,
        bolt,
        enemy_bullet,
        blomb,
        boltaray,
        spawner,
        soldier,
        lurk,
        generator,
        barrier,
        count
    };

    static TaggedObject*& get_taglist(Tag tag);
    static int count(Tag tag);


    explicit TaggedObject(Tag tag) : tag_(tag)
    {
        next_ = get_taglist(tag);
        if (next_) {
            next_->prev_ = this;
        }
        get_taglist(tag) = this;
    }


    TaggedObject(const TaggedObject&) = delete;


    ~TaggedObject()
    {
        auto& tl = get_taglist(tag_);
        if (tl == this) {
            // We're the head of the list! Prev is the new list head.
            tl = next_;
            next_->prev_ = nullptr;
        } else {
            if (prev_) { // Should always exist, as we're not the list head...
                prev_->next_ = next_;
            }
            if (next_) {
                next_->prev_ = prev_;
            }
        }
    }


    TaggedObject* next() const
    {
        return next_;
    }


private:
    TaggedObject* next_ = nullptr;
    TaggedObject* prev_ = nullptr;
    Tag tag_;
};



}
