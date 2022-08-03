if (mapstopfire==0){
if (!ignoreall && global.blaster>0)
    image_single=0;
if (!ignoreall && global.foreverbomb==1 && chargeblade<20){
    global.foreverbomb=0;
    instance_create(x,y+16,obj_foreverbomb2);
    }
if (!ignoreall && global.blaster>0 && pain==0 && chargeblade<20){
    if (global.heat==0){
        if (global.handstand==1)
            hsy=-2;
        else
            hsy=0;
        if (expel==4 && expelcyc>0 && global.hp>1 && argument0==1){
            global.hp-=1;
            expel=0;
            obj_bgdraw.alpha=0.14;
            obj_sabot.explo[3]=1;
            for (i=0;i<32;i+=1){
                tempid=instance_create(x,y+hsy,obj_shot);
                tempid.speed=4;
                tempid.nocount=1;
                tempid.direction=i*11.25;
                tempid=instance_create(x,y+hsy,obj_shot);
                tempid.speed=3;
                tempid.nocount=1;
                tempid.direction=i*11.25;
                }
            }
        else if ((global.autofire && argument0==0) || (argument0==1 && !global.autofire)){
            global.countshots=0;
            with (obj_shot){
                if (!nocount)
                    global.countshots+=1;
                }
            if (global.hypergun && global.countshots<24){
                (instance_create(x-2,y+hsy,obj_shot)).speed=-4;
                (instance_create(x+2,y+hsy,obj_shot)).speed=-4;
                (instance_create(x,y+hsy-2,obj_shot)).speed=-4;
                (instance_create(x,y+hsy+2,obj_shot)).speed=-4;
                if (!shotsound){
                    shotsound=1;
                    scr_sound("shoot");
                    }
                }
            else if (global.blaster==1 && global.countshots<6){
                (instance_create(x-2,y+hsy,obj_shot)).speed=-4;
                if (!shotsound){
                    shotsound=1;
                    scr_sound("shoot");
                    }
                }
            else if (global.blaster==2 && global.countshots<6){
                tempid=instance_create(x,y+hsy,obj_shot);
                tempid.sprite_index=spr_shot2;
                tempid.speed=-4;
                if (!shotsound){
                    shotsound=1;
                    scr_sound("shoot");
                    }
                }
            else if (global.blaster==3 && global.countshots<12){
                (instance_create(x-2,y+hsy,obj_shot)).speed=-4;
                (instance_create(x+2,y+hsy,obj_shot)).speed=-4;
                if (!shotsound){
                    shotsound=1;
                    scr_sound("shoot");
                    }
                }
            }
        }
    }
}