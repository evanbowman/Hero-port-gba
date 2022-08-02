#include "taggedObject.hpp"



namespace herocore
{



TaggedObject*& TaggedObject::get_taglist(Tag tag)
{
    static Buffer<TaggedObject*, (int)Tag::count> tag_lists_;
    while (not tag_lists_.full()) {
        tag_lists_.push_back(nullptr);
    }

    return tag_lists_[(int)tag];
}



}
