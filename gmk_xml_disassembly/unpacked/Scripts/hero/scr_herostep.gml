scr_joy();
shotsound=0;

//STOP FIRING AFTER MAP
if (mapstopfire>0)
    mapstopfire-=1;

//AUTOFIRE
if (global.autofire && chargeblade<6 && chargewarp<6){
    autofire+=1;
    if (autofire>=4){
        autofire=0;
        if (image_single==0)
            scr_fireleft();
        else
            scr_fireright();
        }
    }
else
    autofire=0;

if (!ignoreall && !obj_sabot.pause){
    //EXPEL
    if (keyboard_check(vk_up) || joyhold[34]){
        if (expel==0 || expel==1){
            expelcyc=30;
            expel=1;
            }
        else
            expel=0;
        }
    if ((keyboard_check(vk_down) || joyhold[36]) && expel<=2){
        if (expel==1 || expel==2)
            expel=2;
        else
            expel=0;
        }
    if (keyboard_check(vk_left) || joyhold[35]){
        if (expel==2 || expel==3)
            expel=3;
        else
            expel=0;
        }
    if (keyboard_check(vk_right) || joyhold[33]){
        if (expel==3 || expel==4)
            expel=4;
        else
            expel=0;
        }
    if (expelcyc>0)
        expelcyc-=1;
    //MOVE
    if (chargewarp<20){
        if (keyboard_check(vk_left) || joyhold[35]){
            if (place_free(x-flyspeed,y))
                x-=flyspeed;
            }
        else if (keyboard_check(vk_right) || joyhold[33]){
            if (place_free(x+flyspeed,y))
                x+=flyspeed;
            }
        if (keyboard_check(vk_up) || joyhold[34]){
            if (place_free(x,y-flyspeed))
                y-=flyspeed;
            }
        else if (keyboard_check(vk_down) || joyhold[36]){
            if (place_free(x,y+flyspeed))
                y+=flyspeed;
            }
        }
    }

//CHARGE WARP
if (pain>0 || cannotwarp==1 || global.bossmode || global.forever)
    chargewarp=0;
if (!ignoreall){
    if ((keyboard_check(ord("X")) || joyhold[global.joyx]) && (keyboard_check(ord("Z")) || joyhold[global.joyz]) && chargewarp<80)
        chargewarp+=1;
    else
        chargewarp=0;
    }
else
    chargewarp=0;
if (chargewarp>=53){
    with (obj_sabot)
        scr_warpready();
    }

//CHARGE BLADE - now supports infinite charge while pain/heat
if (chargeblade==20 && chargebladeprev==19)
    scr_sound("chargeblade");
chargebladeprev=chargeblade;
if (!ignoreall && global.blade>0){
    if (keyboard_check(ord("X")) || joyhold[global.joyx]){
        //alarm[1]=20;
        if (!keyboard_check(ord("Z")) && !joyhold[global.joyz] && chargeblade<20){
            chargeblade+=1;
            bladedir=1;
            }
        }
    else if (keyboard_check(ord("Z")) || joyhold[global.joyz]){
        //alarm[1]=20;
        if (!keyboard_check(ord("X")) && !joyhold[global.joyx] && chargeblade<20){
            chargeblade+=1;
            bladedir=0;
            }
        }
    else if (chargeblade==20){
        if (pain==0 && global.heat==0){
            chargeblade=0;
            if (global.foreverbomb){
                global.foreverbomb=0;
                instance_create(x,y+16,obj_foreverbomb2);
                }
            //SPAWN BLADE
            if (image_single==0){
                tempid=instance_create(x-10,y,obj_blade);
                if (global.blade==3)
                    tempid.sprite_index=spr_blade2;
                tempid.image_single=0;
                mask_index=spr_blade;
                with (obj_dirt)
                    solid=0;
                if (global.blade>=2){
                    with (obj_pipe1)
                        solid=0;
                    with (obj_pipe2)
                        solid=0;
                    }
                if (place_free(x-10,y)){
                    tempid=instance_create(x-24,y,obj_blade);
                    if (global.blade==3)
                        tempid.sprite_index=spr_blade2;
                    tempid.image_single=0;
                    }
                mask_index=-1;
                with (obj_dirt)
                    solid=1;
                with (obj_pipe1)
                    solid=1;
                with (obj_pipe2)
                    solid=1;
                scr_sound("blade");
                }
            else{
                tempid=instance_create(x+12,y,obj_blade);
                if (global.blade==3)
                    tempid.sprite_index=spr_blade2;
                tempid.image_single=1;
                mask_index=spr_blade;
                with (obj_dirt)
                    solid=0;
                if (global.blade>=2){
                    with (obj_pipe1)
                        solid=0;
                    with (obj_pipe2)
                        solid=0;
                    }
                if (place_free(x+12,y)){
                    tempid=instance_create(x+26,y,obj_blade);
                    if (global.blade==3)
                        tempid.sprite_index=spr_blade2;
                    tempid.image_single=1;
                    }
                mask_index=-1;
                with (obj_dirt)
                    solid=1;
                with (obj_pipe1)
                    solid=1;
                with (obj_pipe2)
                    solid=1;
                scr_sound("blade");
                }
            }
        }
    else if (pain==0 && global.heat==0)
        chargeblade=0;
    }
else if (pain==0 && global.heat==0)
    chargeblade=0;

//PAIN
if (pain>0)
    pain-=1;
if (superpain>0)
    superpain-=1;

//SCROLL
if (x<obj_sabot.x+4){
    scr_wipescroll();
    x-=30;
    chargewarp=0;
    enterx=x;
    entery=y;
    obj_sabot.x-=320;
    with (obj_sabot)
        scr_scroll();
    obj_sabot.alarm[5]=20;
    }
if (x>obj_sabot.x+316){
    scr_wipescroll();
    x+=30;
    chargewarp=0;
    enterx=x;
    entery=y;
    obj_sabot.x+=320;
    with (obj_sabot)
        scr_scroll();
    obj_sabot.alarm[5]=20;
    }
if (y<obj_sabot.y+8){
    scr_wipescroll();
    y-=30;
    chargewarp=0;
    enterx=x;
    entery=y;
    obj_sabot.y-=320;
    with (obj_sabot)
        scr_scroll();
    obj_sabot.alarm[5]=20;
    }
if (y>obj_sabot.y+312){
    scr_wipescroll();
    y+=30;
    chargewarp=0;
    enterx=x;
    entery=y;
    obj_sabot.y+=320;
    with (obj_sabot)
        scr_scroll();
    obj_sabot.alarm[5]=20;
    }

//FLAME
if (global.heroflames){
    if (x!=xprevious || y!=yprevious){
        flamecycle+=1;
        if (global.handstand==5){
            if (image_single==0){
                tempid=instance_create(x-2,y-6,obj_flame);
                tempid.alarm[0]=10+random(10);
                tempid.depth=-2;
                }
            if (image_single==1){
                tempid=instance_create(x,y-6,obj_flame);
                tempid.alarm[0]=10+random(10);
                tempid.depth=-2;
                }
            }
        if (flamecycle==6){
            flamecycle=0;
            if (global.handstand==1){
                if (image_single==0)
                    instance_create(x+4,y-4,obj_flame);
                if (image_single==1)
                    instance_create(x-6,y-4,obj_flame);
                }
            else if (global.handstand==3){
                instance_create(x+4,y+4,obj_flame);
                instance_create(x-6,y+4,obj_flame);
                }
            else if (global.handstand==6){
                if (image_single==0){
                    instance_create(x-6,y-2,obj_flame);
                    instance_create(x+2,y-2,obj_flame);
                    instance_create(x-2,y-6,obj_flame);
                    }
                if (image_single==1){
                    instance_create(x-4,y-2,obj_flame);
                    instance_create(x+4,y-2,obj_flame);
                    instance_create(x,y-6,obj_flame);
                    }
                }
            else if (global.handstand!=5){
                if (image_single==0)
                    instance_create(x+4,y+2,obj_flame);
                if (image_single==1)
                    instance_create(x-6,y+2,obj_flame);
                }
            }
        }
    }
//LIQUID
if (place_meeting(x,y,obj_liquid1) && global.suit<2){
    global.heat+=3;
    }
if (global.heat>10){
    if (((global.gmversion==5 && obj_bgdraw.alpha>0.40) || (global.gmversion==7 && obj_bgdraw.alpha>0.20)) && obj_bgdraw.alpha<=1)
        obj_bgdraw.alpha-=0.1;
    }

//HEAT REDUCTION
if (global.heat>0 && !pause){
    if (global.suit==0)
        global.heat-=1;
    if (global.suit==1)
        global.heat-=2;
    if (global.suit>=2)
        global.heat=0;
    if (heatscyc>=18){
        heatscyc=0;
        scr_sound("heat");
        }
    }
if (global.heat<0)
    global.heat=0;
if (heatscyc<18)
    heatscyc+=1;

//CRUSHED
if (place_meeting(x,y,obj_block)){
    if (global.hp>4 || (global.hp>2 && global.suit==3)){
        instance_create(x,y,obj_explo);
        x=enterx;
        y=entery;
        }
    scr_heropain(4,0,1,60);
    ignoreall=40;
    alarm[0]=40;
    obj_sabot.explo[1]=1;
    }

//HANDSTAND
if (keyboard_check(ord("Z")) || keyboard_check(ord("X")) || joyhold[global.joyz] || joyhold[global.joyx]){
    if ((keyboard_check(vk_down) || joyhold[36]) && !keyboard_check(vk_up) && !keyboard_check(vk_left) && !keyboard_check(vk_right) && !joyhold[33] && !joyhold[34] && !joyhold[35]){
        if (!place_free(x,y+1)){
            hand+=1;
            if ((global.handstand==0 && hand==200) || (global.handstand>0 && hand==80)){
                hand=0;
                obj_sabot.alarm[2]=200;
                if (global.handstand==0){
                    sprite_index=spr_handstand1;
                    global.handstand=1;
                    obj_sabot.text1=scr_omnitext(382);
                    obj_sabot.text2=scr_omnitext(383);
                    obj_sabot.text3="";
                    }
                else if (global.handstand==1){
                    sprite_index=spr_handstand2;
                    global.handstand=2;
                    obj_sabot.text1=scr_omnitext(95)+scr_omnitext(384);
                    obj_sabot.text2="";
                    obj_sabot.text3="";
                    }
                else if (global.handstand==2){
                    sprite_index=spr_handstand3;
                    global.handstand=3;
                    obj_sabot.text1=scr_omnitext(81)+scr_omnitext(384);
                    obj_sabot.text2="";
                    obj_sabot.text3="";
                    }
                else if (global.handstand==3){
                    sprite_index=spr_handstand4;
                    global.handstand=4;
                    obj_sabot.text1=scr_omnitext(84)+scr_omnitext(384);
                    obj_sabot.text2="";
                    obj_sabot.text3="";
                    }
                else if (global.handstand==4){
                    sprite_index=spr_handstand5;
                    global.handstand=5;
                    obj_sabot.text1=scr_omnitext(114)+scr_omnitext(384);
                    obj_sabot.text2="";
                    obj_sabot.text3="";
                    }
                else if (global.handstand==5 && global.extraspecial=="X"){
                    sprite_index=spr_handstand6;
                    global.handstand=6;
                    obj_sabot.text1=scr_omnitext(385)+scr_omnitext(384);
                    obj_sabot.text2="";
                    obj_sabot.text3="";
                    }
                else{
                    sprite_index=spr_hero;
                    global.handstand=0;
                    obj_sabot.text1=scr_omnitext(113)+scr_omnitext(384);
                    obj_sabot.text2="";
                    obj_sabot.text3="";
                    }
                }
            }
        else
            hand=0;
        }
    else
        hand=0;
    }
else
    hand=0;
