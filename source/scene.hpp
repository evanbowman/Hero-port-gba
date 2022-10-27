

#pragma once

#include "function.hpp"
#include "memory/pool.hpp"
#include "number/numeric.hpp"
#include <memory>


class Platform;


namespace herocore
{


class Scene;


template <typename T> using ScenePtr = std::unique_ptr<T, void (*)(Scene*)>;


ScenePtr<Scene> null_scene();


class Scene
{
public:
    virtual ~Scene(){};


    virtual ScenePtr<Scene> step()
    {
        return null_scene();
    }


    virtual void display()
    {
    }


    virtual void enter(Scene& prev_scene){};


    virtual void exit(Scene& next_scene){};
};


using DeferredScene = Function<16, ScenePtr<Scene>()>;


namespace scene_pool
{


#if defined(__GBA__) or defined(__NDS__)
static constexpr const int max_scene_size = 350;
#else
static constexpr const int max_scene_size = 700;
#endif
static constexpr const int pool_capacity = 3;


using _Pool = Pool<max_scene_size, pool_capacity, 8>;

extern _Pool pool_;


inline void deleter(Scene* scene)
{
    if (scene) {
        scene->~Scene();
        pool_.free(reinterpret_cast<u8*>(scene));
    }
}


template <typename T, typename... Args> ScenePtr<T> alloc(Args&&... args)
{
    static_assert(sizeof(T) <= max_scene_size);
    static_assert(alignof(T) <= pool_.alignment());

    if (auto mem = pool_.alloc()) {
        new (mem) T(std::forward<Args>(args)...);

        return {reinterpret_cast<T*>(mem), deleter};
    }

    return {nullptr, deleter};
}


template <typename S, typename... Args> DeferredScene make_deferred_scene(Args&&... args)
{
    return [args = std::make_tuple(std::forward<Args>(args)...)] {
        return std::apply([](auto&&... args) { return alloc<S>(args...); },
                          std::move(args));
    };
}


} // namespace scene_pool


} // namespace herocore
