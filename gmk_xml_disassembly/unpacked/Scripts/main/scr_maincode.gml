//It's finally back - the return of NEO MODE
if (tier=="main" && !ignoreall){
    if (argument0==0 && kuribo==0)
        kuribo=1;
    else if (argument0==0 && kuribo==1)
        kuribo=2;
    else if (argument0==1 && kuribo==2)
        kuribo=3;
    else if (argument0==1 && kuribo==3)
        kuribo=4;
    else if (argument0==2 && kuribo==4)
        kuribo=5;
    else if (argument0==3 && kuribo==5)
        kuribo=6;
    else if (argument0==2 && kuribo==6)
        kuribo=7;
    else if (argument0==3 && kuribo==7){
        kuribo=8;
        global.languages+=1;
        global.language=global.languages;
        selected=4;
        scr_setlanguage();
        scr_maintexts();
        scr_maindesc();
        scr_sound("save");
        }
    else if (kuribo<8)
        kuribo=0;
    }