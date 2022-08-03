if (drawlogo)
    draw_sprite(spr_herologo,logoimg,50,10);
draw_sprite(spr_pause,0,106,178);
if (blackinit>=320){
    dlcyc+=1;
    if (dlcyc==20)
        drawlogo=1;
    if (dlcyc==30)
        logoimg=1;
    if (dlcyc==40)
        logoimg=2;
    if (dlcyc==50)
        logoimg=3;
    }

//Unlock hidden
/*
if (!ignoreall && tier=="difficulty" && !global.annihilation){
    if (keyboard_check(vk_down) && !keyboard_check(vk_up) && !keyboard_check(vk_left) && !keyboard_check(vk_right)){
        hidden+=1;
        if (hidden==200){
            global.annihilation=1;
            scr_maintexts();
            scr_sound("texttick");
            scr_checkfile();
            if (wrongsum==0)
                scr_savekeys();
            }
        }
    else
        hidden=0;
    }
else
    hidden=0;
*/

if (!ignoreall && tier=="difficulty" && global.annihilation && !global.really){
    if ((keyboard_check(vk_right) || joyhold[global.joyr] || joyhold[33]) && !keyboard_check(vk_left) && !keyboard_check(vk_up) && !keyboard_check(vk_down)){
        hidden2+=1;
        if (hidden2==200){
            global.really=1;
            scr_maintexts();
            scr_sound("texttick");
            }
        }
    else
        hidden2=0;
    }
else
    hidden2=0;

//Description text and cursor
for (i=0;i<13;i+=1)
    scr_drawtext(124,188+i*12,menutext[i],16,400,spr_text,32,1);
if (tier=="copy" && selected>1)
    draw_sprite(spr_text,67,116,200+selected*12);
else if (tier=="copyto" && selected>1)
    draw_sprite(spr_text,11,116,200+selected*12);
else if (tier=="delete" && selected>1)
    draw_sprite(spr_text,88,116,200+selected*12);
else
    draw_sprite(spr_text,30,116,200+selected*12);

if (textcyc<50)
    textcyc+=4;
if (text1!=prevtext1 || text2!=prevtext2 || text3!=prevtext3)
    textcyc=0;
prevtext1=text1;
prevtext2=text2;
prevtext3=text3;
scr_drawtext(4,362,string_copy(text1,0,textcyc),16,400,spr_text,32,1);
scr_drawtext(4,374,string_copy(text2,0,textcyc),16,400,spr_text,32,1);
scr_drawtext(4,386,string_copy(text3,0,textcyc),16,400,spr_text,32,1);

//Fade black
if (fadeblack){
    fadecyc+=1;
    for (i=0;i<25;i+=1){
        (instance_create(fadecyc*20-160,i*32,obj_black1)).depth=-2000;
        (instance_create(fadecyc*20-320,i*32,obj_black1)).depth=-2000;
        (instance_create(fadecyc*20-480,i*32,obj_black1)).depth=-2000;
        (instance_create(400-fadecyc*20,i*32+16,obj_black1)).depth=-2000;
        (instance_create(400-fadecyc*20+160,i*32+16,obj_black1)).depth=-2000;
        (instance_create(400-fadecyc*20+320,i*32+16,obj_black1)).depth=-2000;
        }
    }
//Maps
if (showmap){
    if (map==1)
        draw_sprite(spr_map1,0,200,200);
    else if (map==2)
        draw_sprite(spr_map2,0,200,200);
    else if (map==3)
        draw_sprite(spr_map3,0,200,200);
    }

//First black
if (blackinit<320){
    blackinit+=1;
    for (i=0;i<25;i+=1){
        draw_sprite(spr_black1,0,0,i*16);
        draw_sprite(spr_black1,0,160,i*16);
        draw_sprite(spr_black1,0,320,i*16);
        }
    if (blackinit<314){
        if (yx>0)
            yx=yx/1.2;
        //-------------------------------------------00000000000000000000000000000000000000000000000000-
        scr_drawtext(0,96+(floor(yx*3.0)/2)*2.5+yx2,"         hero core 1.5 - remar games 2010         ",16,400,spr_text,32,1);
        scr_drawtext(0,144+(floor(yx*2.5)/2)*2+yx2, "  design, code, graphics and sfx by daniel remar  ",16,400,spr_text,32,1);
        scr_drawtext(0,192+(floor(yx*2.0)/2)*2+yx2, "             music by brother android             ",16,400,spr_text,32,1);
        scr_drawtext(0,240+(floor(yx*1.5)/2)*2+yx2, "            thanks to sfxr by drpetter            ",16,400,spr_text,32,1);
        scr_drawtext(0,288+(floor(yx*1.0)/2)*2+yx2, "                jbfmod by m. rijks                ",16,400,spr_text,32,1);
        scr_drawtext(0,336+(floor(yx*0.5)/2)*2+yx2, "            thanks to dsu, eo and ages            ",16,400,spr_text,32,1);
        }
    if (blackinit==320){
        textcyc=0;
        ignoreall=0;
        scr_wantsong(11);
        }
    }
//Load text
if (blackinit==1 && !global.musicinit){
    for (i=0;i<25;i+=1){
        draw_sprite(spr_black1,0,0,i*16);
        draw_sprite(spr_black1,0,160,i*16);
        draw_sprite(spr_black1,0,320,i*16);
        }
    scr_drawtext(0,180, "                 loading music...                 ",16,400,spr_text,32,1);
    }
if (blackinit==2)
    scr_createmusic();
