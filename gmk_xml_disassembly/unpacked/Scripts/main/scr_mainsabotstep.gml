if (menubacksound)
    scr_sound("menuback");
menubacksound=0;

//GAMEPAD
if (joywait==20){
    if (tier=="gamepad" && selected>=2 && selected<=10){
        scr_joy(1);
        scr_joyassign();
        }
    else
        scr_joy();
    }
else if (joywait<20)
    joywait+=1;

//SHEEN ETC
if (blackinit>=320 && !nosheen && drawlogo)
    sheen+=1;
if (sheen>sheenmax){
    sheen=0;
    sheenmax=40+random(40);
    temprand=floor(random(10));
    if (temprand==0) instance_create(80,40,obj_sheen);
    else if (temprand==1) instance_create(144,8,obj_sheen);
    else if (temprand==2) instance_create(190,118,obj_sheen);
    else if (temprand==3) instance_create(350,40,obj_sheen);
    else if (temprand==4) instance_create(208,40,obj_sheen);
    else if (temprand==5) instance_create(48,118,obj_sheen);
    else if (temprand==6) instance_create(318,128,obj_sheen);
    else if (temprand==7) instance_create(144,128,obj_sheen);
    else if (temprand==8) instance_create(222,142,obj_sheen);
    else if (temprand==9) instance_create(80,130,obj_sheen);
    }
if (blackinit>=320)
    shooting+=1;
if (shooting>=200+sheenmax*2 && !shooting2){
    shooting2=1;
    tempid=instance_create(400,floor((100+random(50))/2)*2,obj_sheen);
    tempid.shooting=1;
    }
//MUSIC
scr_music();