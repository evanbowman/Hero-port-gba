//Wipe room in Hero Forever
if (global.forever){
    with (obj_enemy){
        if (chestspawn || type==7)
            instance_destroy();
        }
    if (instance_number(obj_enemy)==0){
        obj_sabot.clearedroom[floor(obj_sabot.x/320),floor(obj_sabot.y/320)]=1;
        with (obj_spawner){
            if (!foreverstay)
                instance_destroy();
            }
        }
    }
