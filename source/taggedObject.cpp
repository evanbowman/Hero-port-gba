#include "taggedObject.hpp"


namespace herocore
{


int TaggedObject::count(Tag tag)
{
    int result = 0;
    if (TaggedObject* tl = get_taglist(tag)) {
        while (tl) {
            ++result;
            tl = tl->next();
        }
    }

    return result;
}


TaggedObject*& TaggedObject::get_taglist(Tag tag)
{
    static Buffer<TaggedObject*, (int)Tag::count> tag_lists_;
    while (not tag_lists_.full()) {
        tag_lists_.push_back(nullptr);
    }

    return tag_lists_[(int)tag];
}


} // namespace herocore
