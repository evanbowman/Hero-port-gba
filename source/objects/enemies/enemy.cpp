#include "enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"



namespace herocore
{



bool Enemy::damage(Health dmg, Object& s)
{
    StringBuffer<32> snd("snd_hit");
    snd += to_string<10>(hit_sound());
    play_sound(snd.c_str(), 1);
    health_ = std::max(0, health_ - dmg);
    s.kill();
    return true;
}



}
