//CAST
if (global.cut==4)
    scr_castscroll();

//FIRST TEXT
if (firsttext<40){
    firsttext+=1;
    if (firsttext==40){
        if (global.cut==3)
            scr_wantsong(15);
        ignoreenter=0;
        text1=texts1[0];
        text2=texts2[0];
        text3=texts3[0];
        chat+=1;
        }
    }

//DRAW
if (textcyc<50)
    textcyc+=1;
if (text1!=prevtext1 || text2!=prevtext2 || text3!=prevtext3)
    textcyc=0;
prevtext1=text1;
prevtext2=text2;
prevtext3=text3;
scr_drawtext(4,362,string_copy(text1,0,textcyc),16,400,spr_text,32,1);
scr_drawtext(4,374,string_copy(text2,0,textcyc),16,400,spr_text,32,1);
scr_drawtext(4,386,string_copy(text3,0,textcyc),16,400,spr_text,32,1);
//ARROW
if (chat>0 && global.cut!=4 && string_length(text1)<textcyc && string_length(text2)<textcyc && string_length(text3)<textcyc){
    arrowcyc+=1;
    if (arrowcyc==4){
        arrowcyc=0;
        arrowcyc2+=2;
        }
    if (arrowcyc2>4)
        arrowcyc2=0;
    draw_sprite(spr_text,94,388,388+arrowcyc2);
    }

//TEXT TICK
textscyc+=1;
if (textscyc>=3){
    textscyc=0;
    if (string_length(text1)>textcyc || string_length(text2)>textcyc || string_length(text3)>textcyc)
        scr_sound("texttick");
    }            

//DRAW BLACK
if (drawblack || blackinit<4){
    blackinit+=1;
    for (i=0;i<25;i+=1){
        draw_sprite(spr_black1,0,0,i*16);
        draw_sprite(spr_black1,0,160,i*16);
        draw_sprite(spr_black1,0,320,i*16);
        }
    }