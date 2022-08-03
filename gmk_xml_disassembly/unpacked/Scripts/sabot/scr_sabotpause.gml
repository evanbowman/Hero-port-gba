//Pressing Escape for Pause menu
if (global.hp>0 && blackinit==4 && !cannotpause){
if (!pause){
    if (obj_hero.x>obj_sabot.x+4 && obj_hero.x<obj_sabot.x+316 && obj_hero.y>obj_sabot.y+8 && obj_hero.y<obj_sabot.y+312){
        numrooms=0;
        for (i=0;i<15;i+=1){
            for (j=0;j<15;j+=1){
                if (mroom[i,j]>0)
                    numrooms+=1;
                }
            }
        pause=1;
        resetpickfps=1;
        changedoptions=0;
        scr_sound("pause");
        shakescreen=0;
        instance_deactivate_all(1);
        oldtext1=text1;
        oldtext2=text2;
        oldtext3=text3;
        text1="";
        text2="";
        text3="";
        if (global.bossmode || global.forever)
            tier="bossmode";
        else
            tier="main";
        selected=1;
        scr_pausetexts();
        scr_pausedesc();
        }
    }
//Pause escape
else if (pause){
    if (tier=="main" || tier=="warp" || tier=="bossmode" || (tier=="map" && !mapfrompause)){
        scr_sabot_unpause();
        }
    else if (tier=="information"){
        selected=1;
        scr_pauseselect();
        }
    else if (tier=="options"){
        selected=1;
        scr_pauseselect();
        }
    else if (tier=="gamepad"){
        selected=1;
        scr_pauseselect();
        }
    else if (tier=="savefirst"){
        selected=2;
        scr_pauseselect();
        }
    else if (tier=="quittitle"){
        selected=2;
        scr_pauseselect();
        }
    else if (tier=="quitgame"){
        selected=2;
        scr_pauseselect();
        }
    else if (tier=="map"){
        selected=1;
        scr_pauseselect();
        }
    else if (tier=="retry"){
        selected=2;
        scr_pauseselect();
        }
    }
}
