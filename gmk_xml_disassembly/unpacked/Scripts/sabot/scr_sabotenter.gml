//PAUSE SELECTION
if (global.hp>0 && pause && blackinit==4){
    scr_pauseselect();
    }

//MAP ON/OFF
else if (global.hp>0 && !pause && blackinit==4 && !cannotpause){
if (obj_hero.x>obj_sabot.x+4 && obj_hero.x<obj_sabot.x+316 && obj_hero.y>obj_sabot.y+8 && obj_hero.y<obj_sabot.y+312){
    pause=1;
    mapfrompause=0;
    scr_sound("pause");
    shakescreen=0;
    instance_deactivate_all(1);
    oldtext1=text1;
    oldtext2=text2;
    oldtext3=text3;
    text1="";
    text2="";
    text3="";
    tier="map";
    selected=1;
    scr_pausetexts();
    scr_pausedesc();
    }
    }