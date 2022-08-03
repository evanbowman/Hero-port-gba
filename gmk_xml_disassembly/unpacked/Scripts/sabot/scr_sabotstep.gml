//LAG DETECT
if (resetpickfps){
    resetpickfps=0;
    for (i=0;i<5;i+=1)
        pickfps[i]=1;
    }
if (global.motionblur && !pause){
    if (instance_number(obj_bgdraw)==1){
        if (obj_bgdraw.alpha<1 && obj_bgdraw.alpha>0){
            pickfpscyc+=1;
            if (pickfpscyc==20){
                pickfpscyc=0;
                slowdowns=0;
                for (i=0;i<4;i+=1){
                    pickfps[i]=pickfps[i+1];
                    if (pickfps[i]>540)
                        slowdowns+=1;
                    }
                pickfps[4]=current_time-curtimeprev;
                curtimeprev=current_time;
                if (slowdowns>=4 || (global.gmversion==5 && slowdowns>=2)){
                    for (i=0;i<5;i+=1)
                        pickfps[i]=1;
                    global.motionblur=0;
                    }
                }
            }
        else
            curtimeprev=current_time;
        }
    }

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

//SOUNDS
if (fire[0]) scr_sound("fireshot");
if (fire[1]) scr_sound("firesuper");
if (fire[2]) scr_sound("firemega");
if (fire[3]) scr_sound("firegiga");
if (fire[4]) scr_sound("firebomb");
if (fire[5]) scr_sound("firemissile");
if (fire[6]) scr_sound("firevortex");
if (fire[7]) scr_sound("firelaser");
if (fire[8]) scr_sound("charge");
if (explo[0]) scr_sound("ping");
if (explo[1]) scr_sound("explo1");
if (explo[2]) scr_sound("explo2");
if (explo[3]) scr_sound("explo3");
if (explo[4]) scr_sound("explo4");
if (hit[1]) scr_sound("hit1");
if (hit[2]) scr_sound("hit2");
if (hit[3]) scr_sound("hit3");
if (hit[4]) scr_sound("hit4");
if (menubacksound) scr_sound("menuback");
for (i=0;i<9;i+=1)
    fire[i]=0;
for (i=0;i<5;i+=1)
    explo[i]=0;
for (i=0;i<5;i+=1)
    hit[i]=0;
menubacksound=0;

//MDK
if (mdk){
    global.hp-=0.1;
    if (global.hp<0){
        if (!mdkscream){
            mdkscream=1;
            scr_sound("death");
            }
        global.hp=0;
        }
    }
//TIME
if (!pause && !cannotpause && global.hp>0 && !obj_hero.ignoreall && !(global.bossmode && roomx<=5) && !(global.bossmode && roomx==6 && roomy==9))
    timetick+=1;
if (timetick==40){
    timetick=0;
    global.seconds+=1;
    }
if (global.seconds>359998)
    global.second=359998;

//HEAT LIMITS
if (global.heat>global.maxheat)
    global.heat=global.maxheat;
if (global.heat<0 || global.suit>=2 || global.hp==0)
    global.heat=0;

//OPEN DOORS
if (!pause && global.hp>0 && (instance_number(obj_enemy)==0 || instance_number(obj_enemy)==instance_number(obj_generator))){
    if (instance_number(obj_pickup)==1)
        pickuppresent=1;
    if (instance_number(obj_pickup)==0){
        mdoorclear[roomx,roomy]=1;
        temp=0;
        with (obj_door1){
            instance_destroy();
            instance_create(x+8,y+8,obj_explo);
            if (type==0)
                instance_create(x+8,y+24,obj_explo);
            else
                instance_create(x+24,y+8,obj_explo);
            obj_sabot.temp=1;
            }
        if (pickuppresent && global.difficulty!=2)
            temp=0;
        pickuppresent=0;
        if (global.bossmode)
            temp=0;
        if (temp){
            text1=scr_omnitext(478);
            text2=scr_omnitext(479);
            text3="";
            }
        }
    }

//BLACK CIRCLE
if (blackcircle<400 && blackinit==4 && !pause){
    blackcircle+=8;
    with (obj_blackcircle){
        if (point_distance(obj_hero.x,obj_hero.y,x+16,y+16)<obj_sabot.blackcircle)
            instance_destroy();
        }
    if (blackcircle>=400)
        cannotpause=0;
    }

//SHAKESCREEN
if (shakescreen>0)
    shakescreen-=1;
if (global.screenshaking)
    shake=(shakescreen mod 2)*2;
else
    shake=0;
if (global.gmversion==5){
    view_left[0]=x-40;
    view_top[0]=y-40+shake;
    y2=view_top[0]+40;
    }
else{
    view_xview[0]=x-40;
    view_yview[0]=y-40+shake;
    y2=view_yview[0]+40;
    }

//BLADED ENEMIES
with (obj_enemy)
    bladed=0;

//MUSIC
scr_music();
