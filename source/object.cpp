#include "object.hpp"



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



}
