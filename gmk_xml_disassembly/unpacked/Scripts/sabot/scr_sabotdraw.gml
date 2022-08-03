//DRAW TETRON CIRCLE
if (global.gmversion==7 && drawtetroncircle)
    scr_compat(6);

//SHOWHP
if (global.showhp){
    thevar=0;
    with (obj_enemy){
        if (drawhp && hp>0 && !(type==38 && hp>128)){
            thevar=hp;
            if (obj_guardian.drain>3 && obj_guardian.hp<=0 && obj_guardian.hp2>0 && type==37)
                thevar=obj_guardian.hp2;
            if (thevar>0 && thevar<1)
                thevar=1;
            if (thevar<10)
                draw_sprite(spr_text,floor(thevar mod 10)+16,floor(round(bbox_left/2)*2),floor(round((bbox_top-12)/2)*2));
            else if (thevar<100){
                draw_sprite(spr_text,floor(thevar/10)+16,floor(round(bbox_left/2)*2),floor(round((bbox_top-12)/2)*2));
                draw_sprite(spr_text,floor(thevar mod 10)+16,floor(round(bbox_left/2)*2)+8,floor(round((bbox_top-12)/2)*2));
                }
            else{
                draw_sprite(spr_text,floor(thevar/100)+16,floor(round(bbox_left/2)*2),floor(round((bbox_top-12)/2)*2));
                draw_sprite(spr_text,floor((thevar mod 100)/10)+16,floor(round(bbox_left/2)*2)+8,floor(round((bbox_top-12)/2)*2));
                draw_sprite(spr_text,floor(thevar mod 10)+16,floor(round(bbox_left/2)*2)+16,floor(round((bbox_top-12)/2)*2));
                }
            }
        }
    with (obj_hero){
        if (global.hp>0){
            thevar=global.hp;
            if (thevar>0 && thevar<1)
                thevar=1;
            if (thevar<10)
                draw_sprite(spr_text,floor(thevar mod 10)+16,floor(round(bbox_left/2)*2),floor(round((bbox_top-12)/2)*2));
            else{
                draw_sprite(spr_text,floor(thevar/10)+16,floor(round(bbox_left/2)*2),floor(round((bbox_top-12)/2)*2));
                draw_sprite(spr_text,floor(thevar mod 10)+16,floor(round(bbox_left/2)*2)+8,floor(round((bbox_top-12)/2)*2));
                }
            }
        }
    }

//DRAW HUD
if (y!=y2){
    if (zonenum==22){
        draw_sprite(spr_blockwide3,0,x,y2+318);
        draw_sprite(spr_blockwide3,0,x+160,y2+318);
        }
    else{
        draw_sprite(spr_black1,0,x,y2+318);
        draw_sprite(spr_black1,0,x+160,y2+318);
        }
    }
draw_sprite(spr_hud1,0,x-40,y2-40);
draw_sprite(spr_hud1,0,x-40,y2+320);
draw_sprite(spr_hud2,0,x-40,y2+18);
draw_sprite(spr_hud2,0,x+320,y2+18);
draw_sprite(spr_hud3,0,x-40,y2);
draw_sprite(spr_hud3,1,x+320,y2);
barlen=floor(((global.hp/global.maxhp)*292)/4)*4;
draw_sprite_part(spr_hudbar,0,0,0,16,barlen,x-28,y2+18+292-barlen);
if (global.level<10)
    draw_sprite(spr_text,16+global.level,x-12,y2+6);
else
    draw_sprite(spr_text,88,x-12,y2+6);
barlen=floor(((global.heat/global.maxheat)*292)/4)*4;
draw_sprite_part(spr_hudbar,0,0,0,16,barlen,x+332,y2+18+292-barlen);

//TEXT BLINK
if (textcyc<50){
    if (pause)
        textcyc+=4;
    else{
        textcyc+=1;
        if (textcyc<20 && text1!="" && textcyc mod 8<4){
            draw_sprite(spr_hudline1,0,x-40,y2+320);
            draw_sprite(spr_hudline1,0,x-40,y2+358);
            draw_sprite(spr_hudline2,0,x-40,y2+322);
            draw_sprite(spr_hudline2,0,x+358,y2+322);
            }
        }
    }
if (text1!=prevtext1 || text2!=prevtext2 || text3!=prevtext3){
    if (goaroundtextcyc)
        goaroundtextcyc=0;
    else
        textcyc=0;
    }
prevtext1=text1;
prevtext2=text2;
prevtext3=text3;

//HUD ADDONS
if (global.bossmode || global.forever){
    scr_drawtext(x-36,y2-32,extstring1,10,400,spr_text,32,1);
    scr_drawtext(x-36,y2-18,extstring3,10,400,spr_text,32,1);
    }
else{
    scr_drawtext(x-36,y2-32,extstring1,10,400,spr_text,32,1);
    scr_drawtext(x-36,y2-18,extstring2,10,400,spr_text,32,1);
    }
scr_drawtext(x+326,y2+6,extstring4,10,400,spr_text,32,1);//heat
scr_drawtext(x-36,y2+6,extstring5,10,400,spr_text,32,1);//lvl

if (!global.bossmode && !global.forever){
    draw_sprite(spr_text,global.fireleft,x-36,y2-18);
    draw_sprite(spr_text,global.fireright,x-20,y2-18);
    }
if (!global.retromode && global.difficulty!=2)
    draw_sprite(spr_hudmap,0,x+44,y2-34);
hudmapcyc+=1;
if (hudmapcyc==16)
    hudmapcyc=0;
if (hudmapcyc>=8 && !global.retromode && global.difficulty!=2 && !skipdraw)
    draw_sprite(spr_hudmap2,0,x+42+8*floor(roomx/3),y2-36+6*floor(roomy/3));
if (zonecyc<50)
    zonecyc+=0.5;
if (zone!=prevzone)
    zonecyc=0;
prevzone=zone;
scr_drawtext(x+88,y2-32,string_copy(zone,0,ceil(zonecyc)),10,400,spr_text,32,1);
if (global.bossmode){
    scr_timestring();
    scr_drawtext(x+88,y2-18,timestring,10,400,spr_text,32,1);
    scr_drawtext(x+252-string_length(extstring8)*8,y2-32,extstring8,10,400,spr_text,32,1);
    scr_drawtext(x+188,y2-18,besttimestr,10,400,spr_text,32,1);
    }
else if (global.forever){
    scr_drawtext(x+56,y2-18,extstring6+" "+string(foreverlevel),10,400,spr_text,32,1);
    //scr_drawtext(x+136,y2-18,string(foreverlevel),10,400,spr_text,32,1);
    scr_drawtext(x+160,y2-18,extstring7+" "+string(flags),10,400,spr_text,32,1);
    //scr_drawtext(x+216,y2-18,string(flags),10,400,spr_text,32,1);
    }
else
    scr_drawtext(x+88,y2-18,string_copy(zone2,0,ceil(zonecyc)),10,400,spr_text,32,1);

//HUD BOSS
if ((global.retromode || boss || global.difficulty==2) && !global.forever && !global.really){
    draw_sprite(spr_hudboss,0,x+36,y2-34);
    scr_bosslen();
    if (bossdraw mod 4 <2)
        draw_sprite_part(spr_hudboss2,0,0,0,bosslen2,4,x+40,y2-14);
    }
else{
    bosslen=1;
    bosslenprev=2;
    }

//HUD ITEMS
if (global.blaster>0){
    draw_sprite(spr_huditems,0,x+256,y2-34);
    if (global.hypergun)
        draw_sprite(spr_text,88,x+278,y2-16)
    else
        draw_sprite(spr_text,16+global.blaster,x+278,y2-16)
    if (global.autofire)
        draw_sprite(spr_hudhold,0,x+256,y2-36);
    }
if (global.blade>0){
    draw_sprite(spr_huditems,1,x+292,y2-34);
    draw_sprite(spr_text,16+global.blade,x+314,y2-16)
    }
if (global.suit>0){
    draw_sprite(spr_huditems,2,x+328,y2-34);
    draw_sprite(spr_text,16+global.suit,x+350,y2-16)
    }
if (mdk){
    if (global.blaster>0)
        draw_sprite(spr_text,16,x+278,y2-16)
    if (global.blade>0)
        draw_sprite(spr_text,16,x+314,y2-16)
    if (global.suit>0)
        draw_sprite(spr_text,16,x+350,y2-16)
    }
if (global.computers>0){
    draw_sprite(spr_hudcomputer,0,x+220,y2-34);
    if (global.computers==10)
        draw_sprite(spr_text,88,x+242,y2-16)
    else
        draw_sprite(spr_text,16+global.computers,x+242,y2-16)
    }

//BLACK TETRON
if (tetronblackcover){
    draw_sprite(spr_totalcover,0,x-40,y2-40);
    barlen=floor(((global.hp/global.maxhp)*292)/4)*4;
    draw_sprite_part(spr_hudbar,0,0,0,16,barlen,x-28,y2+18+292-barlen);
    scr_drawtext(x-36,y2+6,extstring5,10,400,spr_text,32,1);//lvl
    if (global.level<10)
        draw_sprite(spr_text,16+global.level,x-12,y2+6);
    else
        draw_sprite(spr_text,88,x-12,y2+6);
    }

//HUD TEXT AND INFORMATION BLINKS
scr_drawtext(x-36,y2+322,string_copy(text1,0,textcyc),16,400,spr_text,32,1);
scr_drawtext(x-36,y2+334,string_copy(text2,0,textcyc),16,400,spr_text,32,1);
scr_drawtext(x-36,y2+346,string_copy(text3,0,textcyc),16,400,spr_text,32,1);
if (pause && tier=="information"){
    hudblink+=1;
    if (hudblink==8)
        hudblink=0;
    if (hudblink>3 && !tetronblackcover){
        if (selected==3)
            draw_sprite(spr_hudblink2,0,x-38,y2+4);
        else if (selected==4)
            draw_sprite(spr_hudblink3,0,x-38,y2-20);
        else if (selected==5)
            draw_sprite(spr_hudblink,0,x+254,y2-36);
        else if (selected==6 && global.blade>0)
            draw_sprite(spr_hudblink,0,x+290,y2-36);
        else if (selected==7 && global.suit>0)
            draw_sprite(spr_hudblink,0,x+326,y2-36);
        else if (selected==8 && global.computers>0)
            draw_sprite(spr_hudblink,0,x+218,y2-36);
        }
    }

//TEXT TICK
if (!pause)
textscyc+=1;
if (textscyc>=3){
    textscyc=0;
    if (string_length(text1)>textcyc || string_length(text2)>textcyc || string_length(text3)>textcyc || string_length(zone)>zonecyc || (string_length(zone2)>zonecyc && !global.bossmode && !global.forever))
            scr_sound("texttick");
    }

//DRAW MAP
if (pause && (tier="map" || tier=="warp")){
    draw_sprite(spr_map,0,x,y);
    //Black map
    if (global.retromode || global.difficulty==2){
        for (i=0;i<19;i+=1){
            draw_sprite(spr_black1,0,x,y+i*16+2);
            draw_sprite(spr_black1,0,x+160,y+i*16+2);
            }
        draw_sprite(spr_black1,0,x,y+302);
        draw_sprite(spr_black1,0,x+160,y+302);
        }
    mapcyc+=1;
    if (mapcyc==12)
        mapcyc=0;
    //Shift map in Forever mode
    if (global.forever){
        mapshiftx=148-masterxsize*10;
        mapshifty=152-masterysize*10;
        }
    else{
        mapshiftx=0;
        mapshifty=0;
        }
    //Draw dots in Forever mode
    if (!global.retromode && global.forever){
        for (i=0;i<masterxsize;i+=1){
            for (j=0;j<masterysize;j+=1)
                draw_sprite(spr_mapicons,23,x+20*i+10+mapshiftx,y+20*j+10+mapshifty);
            }
        }
    //Detailed room icons
    if (!global.retromode){
        for (i=0;i<15;i+=1){
            for (j=0;j<15;j+=1){
                if (mroom[i,j]>0)
                    draw_sprite(spr_mapicons,0,x+20*i+10+mapshiftx,y+20*j+10+mapshifty);
                }
            }
    //Crossed out rooms in Forever mode
    if (!global.retromode && global.forever){
        for (i=0;i<15;i+=1){
            for (j=0;j<15;j+=1){
                if (clearedroom[i,j]>0 && !(mitem[i,j]==2 && mitemclear[i,j]==0) && !(i==roomx && j==roomy))
                    draw_sprite(spr_mapicons,24,x+20*i+10+mapshiftx,y+20*j+10+mapshifty);
                }
            }
        }
    //Zone borders
    if (global.difficulty!=2)
        draw_sprite(spr_mapoverlay,0,x+10+mapshiftx,y+10+mapshifty);
        }
    //Items and bosses
    for (i=0;i<15;i+=1){
        for (j=0;j<15;j+=1){
            if (mroom[i,j]>0 && mroom[i,j]<16)
                draw_sprite(spr_mapicons,mroom[i,j],x+20*i+10+mapshiftx,y+20*j+10+mapshifty);
            if (mapcyc>=6){
                if (mitem[i,j]==1)
                    draw_sprite(spr_mapicons,16,x+20*i+10+mapshiftx,y+20*j+10+mapshifty);
                else if (mitem[i,j]==2 && mitemclear[i,j]==0)
                    draw_sprite(spr_mapicons,17,x+20*i+10+mapshiftx,y+20*j+10+mapshifty);
                }
            }
        }
    //Hint
    if (mapcyc>=6 && global.showhint && global.difficulty!=2 && !global.bossmode && !global.forever && !global.reallyplay){
        for (i=0;i<10;i+=1){
            if (mitemclear[hintorderx[i],hintordery[i]]==0){
                draw_sprite(spr_mapicons,22,x+20*hintorderx[i]+10+mapshiftx,y+20*hintordery[i]+10+mapshifty);
                i=10;
                }
            }
        }
    //Hero position
    if (mapcyc>=6 && roomx<15 && roomy<15)
        draw_sprite(spr_mapicons,18,x+20*roomx+10+mapshiftx,y+20*roomy+10+mapshifty);
    //Warp selection icon
    if (tier=="warp"){
        if (mapcyc mod 3==0)
            hudwarpimg+=1;
        if (hudwarpimg==3)
            hudwarpimg=0;
        draw_sprite(spr_mapicons,19+hudwarpimg,x+20*warpx+10+mapshiftx,y+20*warpy+10+mapshifty);
        }
    //"TETRON"
    if (global.difficulty<2 && global.oncespecialtext=="!"){
        scr_drawtext(x+136+mapshiftx,y+140+mapshifty,middletext,16,400,spr_text,32,1);
        }
    }

//DRAW PAUSE MENU
if (pause && tier!="map" && tier!="warp"){
    draw_sprite(spr_pause,0,x+66,y+72);
    for (i=0;i<13;i+=1)
        scr_drawtext(x+84,y+82+i*12,menutext[i],16,400,spr_text,32,1);
    draw_sprite(spr_text,30,x+76,y+94+selected*12);
    }

//First black
if (blackinit<4){
    blackinit+=1;
    for (i=0;i<26;i+=1){
        draw_sprite(spr_black1,0,x-40,y-40+i*16);
        draw_sprite(spr_black1,0,x-40+120,y-40+i*16);
        draw_sprite(spr_black1,0,x-40+280,y-40+i*16);
        }
    }

//FLASH
if ((global.hp<prevhp && !mdk && global.screenshaking) || fullflash){
    for (i=0;i<20;i+=1){
        draw_sprite(spr_blockwide3,0,x,y+i*16);
        draw_sprite(spr_blockwide3,0,x+160,y+i*16);
        }
    }
prevhp=global.hp;
