//Intro
if (global.cut==1){
    if (chat==0){
        tempid=instance_create(180,100,obj_cuthelper);
        tempid.sprite_index=spr_cut1;
        tempid.hspeed=0.2;
        tempid=instance_create(74,142,obj_cuthelper);
        tempid.sprite_index=spr_cut2;
        tempid.hspeed=-0.8;
        order=1;
        alarm[1]=88;
        }
    else if (chat==3){
        with (obj_cuthelper)
            instance_destroy();
        for (i=0;i<60;i+=1)
            (instance_create(random(400),random(200)+98,obj_star)).visible=0;
        tempid=instance_create(146,146,obj_cuthelper);
        tempid.sprite_index=spr_cut3;
        tempid.visible=0;
        order=3;
        alarm[1]=4;
        }
    else if (chat==4){
        tempid=instance_create(408,260,obj_cuthelper);
        tempid.sprite_index=spr_cut4;
        tempid.direction=158;
        tempid.speed=1.3;
        tempid.gravity=0.002;
        tempid.type=1;
        }
    }
//Victory
if (global.cut==2){
    if (chat==0){
        order=1;
        alarm[1]=80;
        tempid=instance_create(84,100,obj_cuthelper);
        tempid.sprite_index=spr_cut6;
        tempid.hspeed=-0.8;
        tempid=instance_create(28,138,obj_cuthelper);
        tempid.sprite_index=spr_cut7;
        tempid.hspeed=0.8;
        tempid=instance_create(200,220,obj_cuthelper);
        tempid.sprite_index=spr_cut8;
        tempid.hspeed=0.8;
        tempid.type=5;
        }
    else if (chat==3){
        with (obj_cuthelper)
            instance_destroy();
        for (i=0;i<60;i+=1)
            (instance_create(random(400),random(200)+98,obj_star)).visible=0;
        tempid=instance_create(146,146,obj_cuthelper);
        tempid.sprite_index=spr_cut3;
        tempid.visible=0;
        order=3;
        alarm[1]=4;
        tempid=instance_create(170,230,obj_cuthelper);
        tempid.sprite_index=spr_cut9;
        tempid.direction=200;
        tempid.speed=0.8;
        tempid.gravity_direction=tempid.direction;
        tempid.gravity=0.01;
        tempid.type=6;
        tempid.visible=0;
        }
    }
//Perfect victory
if (global.cut==3){
    if (chat==0){
        for (i=0;i<60;i+=1)
            instance_create(random(400),random(200)+98,obj_star);
        tempid=instance_create(146,146,obj_cuthelper);
        tempid.sprite_index=spr_cut3;
        tempid.type=4;
        firsttext=-140;
        order=2;
        alarm[1]=60;
        }
    }
//Cast (nothing)
if (global.cut==4){}
//Hidden intro
if (global.cut==5){
    if (chat==0){
        for (i=0;i<60;i+=1)
            instance_create(random(400),random(200)+98,obj_star);
        tempid=instance_create(200,200,obj_cuthelper);
        tempid.sprite_index=spr_cut10;
        tempid=instance_create(408,260,obj_cuthelper);
        tempid.sprite_index=spr_cut4;
        tempid.direction=158;
        tempid.speed=1.3;
        tempid.gravity=0.002;
        tempid.type=1;
        }
    }
//Hidden outro
if (global.cut==6){
    if (chat==0){
        tempid=instance_create(228,158,obj_cuthelper);
        tempid.sprite_index=spr_cut11;
        tempid.hspeed=-1;
        tempid=instance_create(80,124,obj_cuthelper);
        tempid.sprite_index=spr_cut12;
        tempid.hspeed=0.2;
        order=1;
        alarm[1]=100;
        }
    if (chat==4){
        with (obj_cuthelper)
            instance_destroy();
        for (i=0;i<60;i+=1)
            (instance_create(random(400),random(200)+98,obj_star)).visible=0;
        tempid=instance_create(200,200,obj_cuthelper);
        tempid.sprite_index=spr_cut10;
        tempid.visible=0;
        order=3;
        alarm[1]=4;
        tempid=instance_create(180,200,obj_cuthelper);
        tempid.sprite_index=spr_cut9;
        tempid.direction=200;
        tempid.speed=0.8;
        tempid.gravity_direction=tempid.direction;
        tempid.gravity=0.01;
        tempid.type=6;
        tempid.visible=0;
        }
    else if (chat==5){
        with (obj_cuthelper)
            instance_destroy();
        with (obj_star)
            instance_destroy();
        for (i=0;i<60;i+=1)
            (instance_create(random(400),random(200)+98,obj_star)).visible=0;
        tempid=instance_create(146,146,obj_cuthelper);
        tempid.sprite_index=spr_cut3;
        tempid.visible=0;
        order=3;
        alarm[1]=4;
        tempid=instance_create(408,260,obj_cuthelper);
        tempid.sprite_index=spr_cut4;
        tempid.direction=158;
        tempid.speed=1.3;
        tempid.gravity=0.002;
        tempid.type=1;
        }
    }
