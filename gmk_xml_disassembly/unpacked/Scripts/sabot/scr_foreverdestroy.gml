global.seconds=0;
obj_sabot.newforeverlevel=1;
instance_activate_all();
with (obj_liquid1)
    instance_destroy();
with (obj_liquid2)
    instance_destroy();
with (obj_liquid1long_s)
    instance_destroy();
with (obj_liquid2big_s)
    instance_destroy();
with (obj_explospewer3)
    instance_destroy();
with (obj_shipscan_s)
    instance_destroy();
with (obj_flag)
    instance_destroy();
with (obj_chest)
    instance_destroy();
with (obj_flutter)
    instance_destroy();
with (obj_spawner)
    instance_destroy();
with (obj_dirtbig_s)
    instance_destroy();
with (obj_block)
    instance_destroy();
with (obj_loc)
    instance_destroy();
with (obj_forevergoal)
    instance_destroy();
with (obj_pickup)
    instance_destroy();
with (obj_enemy)
    instance_destroy();
with (obj_deadanni)
    instance_destroy();
with (obj_shot)
    instance_destroy();
with (obj_flame)
    instance_destroy();
with (obj_eshot)
    instance_destroy();
with (obj_evortex)
    instance_destroy();
with (obj_foreverbomb)
    instance_destroy();
with (obj_foreverbomb2)
    instance_destroy();
with (obj_terror_s)
    instance_destroy();
instance_create(obj_sabot.x,obj_sabot.y,obj_black1);
instance_create(obj_sabot.x+160,obj_sabot.y,obj_black1);
instance_create(obj_sabot.x,obj_sabot.y+304,obj_black1);
instance_create(obj_sabot.x+160,obj_sabot.y+304,obj_black1);
instance_create(obj_sabot.x,obj_sabot.y,obj_black2);
instance_create(obj_sabot.x,obj_sabot.y+160,obj_black2);
instance_create(obj_sabot.x+304,obj_sabot.y,obj_black2);
instance_create(obj_sabot.x+304,obj_sabot.y+160,obj_black2);
