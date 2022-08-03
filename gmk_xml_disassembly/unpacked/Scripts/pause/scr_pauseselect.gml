//MAIN
if (tier="main"){
    if (selected==1){
        scr_sabotpause();
        skipdesc=1;
        menubacksound=1;
        }
    else if (selected==2){
        tier="map";
        mapfrompause=1;
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==3){
        tier="information";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==4){
        tier="options";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==5){
        tier="gamepad";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==6 && !global.reallyplay){
        scr_checkfile();
        if (wrongsum){
            text1=scr_omnitext(551);
            text2=scr_omnitext(552);
            text3=scr_omnitext(553);
            skipdesc=1;
            menubacksound=1;
            }
        else{
            scr_save();
            scr_sound("save");
            text1=scr_omnitext(554);
            text2="";
            text3="";
            skipdesc=1;
            }
        }
    else if (selected==7){
        if (global.reallyplay){
            scr_backtotitle();
            }
        else{
            tier="savefirst";
            selected=1;
            quittype=0;
            }
        scr_sound("menuselect");
        }
    else if (selected==8){
        if (global.reallyplay){
            scr_endgame();
            }
        else{
            tier="savefirst";
            selected=1;
            quittype=1;
            }
        scr_sound("menuselect");
        }
    }

//INFORMATION
else if (tier=="information"){
    if (selected==1){
        tier="main";
        selected=3;
        menubacksound=1;
        }
    }

//OPTIONS
else if (tier="options"){
    changedoptions=1;
    if (selected==1){
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        if (global.bossmode || global.forever){
            tier="bossmode";
            selected=4;
            }
        else{
            tier="main";
            selected=4;
            }
        scr_sound("menuselect");
        }
    else if (selected==4){
        global.sound-=1;
        if (global.sound<0)
            global.sound=4;
        scr_volume();
        scr_sound("menuselect");
        }
    else if (selected==5){
        global.music-=1;
        if (global.music<0)
            global.music=4;
        scr_sound("menuselect");
        jbfmod_set_master_volume(64*global.music);
        if (global.music==0){
            jbfmod_stop_song();
            global.fadesong=0;
            }
        else if (global.music==4)
            scr_wantsong(global.currentsong);
        }
    else if (selected==6){
        if (global.window==0)
            global.window=1;
        else if (global.window==1)
            global.window=2;
        else
            global.window=0;
        scr_compat(1);
        scr_sound("menuselect");
        }
    else if (selected==7){
        if (global.motionblur)
            global.motionblur=0;
        else
            global.motionblur=1;
        scr_sound("menuselect");
        }
    else if (selected==8){
        if (global.screenshaking)
            global.screenshaking=0;
        else
            global.screenshaking=1;
        scr_sound("menuselect");
        }
    else if (selected==9){
        if (global.heroflames)
            global.heroflames=0;
        else
            global.heroflames=1;
        scr_sound("menuselect");
        }
    else if (selected==10){
        if (global.retromode)
            global.retromode=0;
        else
            global.retromode=1;
        scr_sound("menuselect");
        }
    else if (selected==11){
        if (global.showhint)
            global.showhint=0;
        else
            global.showhint=1;
        scr_sound("menuselect");
        }
    }

//GAMEPAD
else if (tier="gamepad"){
    changedoptions=1;
    if (selected==1){
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        if (global.bossmode || global.forever){
            tier="bossmode";
            selected=5;
            }
        else{
            tier="main";
            selected=5;
            }
        scr_sound("menuselect");
        }
    }

//SAVE FIRST
else if (tier=="savefirst"){
    if (selected==1){
        scr_checkfile();
        if (wrongsum){
            text1=scr_omnitext(551);
            text2=scr_omnitext(552);
            text3=scr_omnitext(553);
            menubacksound=1;
            }
        else{
            scr_save();
            scr_sound("save");
            text1=scr_omnitext(554);
            text2="";
            text3="";
            }
        if (quittype==0)
            tier="quittitle";
        else
            tier="quitgame";
        }
    else if (selected==2){
        if (quittype==0)
            tier="quittitle";
        else
            tier="quitgame";
        text1=scr_omnitext(555);
        text2="";
        text3="";
        selected=1;
        menubacksound=1;
        }
    }

//QUIT TO TITLE
else if (tier=="quittitle"){
    if (selected==1){
        scr_backtotitle();
        }
    else if (selected==2){
        if (global.bossmode || global.forever){
            tier="bossmode";
            selected=6;
            }
        else{
            tier="main";
            selected=7;
            }
        menubacksound=1;
        }
    }

//QUIT GAME
else if (tier=="quitgame"){
    if (selected==1){
        scr_endgame();
        }
    else if (selected==2){
        if (global.bossmode || global.forever){
            tier="bossmode";
            selected=7;
            }
        else{
            tier="main";
            selected=8;
            }
        menubacksound=1;
        }
    }
//MAP
else if (tier="map"){
    if (mapfrompause){
        if (global.bossmode || global.forever){
            tier="bossmode";
            selected=3;
            }
        else{
            tier="main";
            selected=2;
            }
        }
    else{
        scr_sabotpause();
        skipdesc=1;
        }
    menubacksound=1;
    }
//WARP
else if (tier="warp"){
    pause=0;
    instance_activate_region(x,y,310,319,1);
    instance_activate_object(obj_deleteme);
    instance_activate_object(obj_blur);
    roomx=warpx;
    roomy=warpy;
    x=roomx*320;
    y=roomy*320;
    respawnroomx=roomx;
    respawnroomy=roomy;
    with (obj_hero)
        instance_destroy();
    obj_bgdraw.alpha=0.1;
    scr_scrollprepare();
    //Old
    /*with (obj_deleteme)
        instance_destroy();
    obj_bgdraw.x=x;
    obj_bgdraw.y=y;
    with (obj_black3)
        solid=1;
    obj_sabot.alarm[2]=-1;*/
    //Deactivate surrounding world
    instance_deactivate_all(1);
    scr_scroll();
    respawnroomx=roomx;
    respawnroomy=roomy;
    if (instance_number(obj_savepoint_s)==1)
        instance_create(obj_savepoint_s.x+16,obj_savepoint_s.y-32,obj_hero);
    else
        instance_create(x+160,y+160,obj_hero);
    for (i=0;i<36;i+=1){
        tempid=instance_create(obj_hero.x,obj_hero.y,obj_explo);
        tempid.gravity_direction=i*10;
        tempid.gravity=6;
        }
    text1="";
    text2="";
    text3="";
    skipdesc=1;
    pickuppresent=0;
    global.hp=global.maxhp;
    }

//BOSSMODE
else if (tier="bossmode"){
    if (selected==1){
        scr_sabotpause();
        skipdesc=1;
        menubacksound=1;
        }
    else if (selected==2){
        tier="retry";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==3){
        tier="map";
        mapfrompause=1;
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==4){
        tier="options";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==5){
        tier="gamepad";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==6){
        tier="quittitle";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==7){
        tier="quitgame";
        selected=1;
        scr_sound("menuselect");
        }
    }

//RETRY
else if (tier=="retry"){
    if (selected==1){
        scr_sabot_unpause();
        skipdesc=1;
        menubacksound=1;
        with (obj_hero)
            scr_heropain(100,0,1,0);
        }
    else if (selected==2){
        tier="bossmode";
        selected=2;
        menubacksound=1;
        }
    }

scr_pausetexts();
if (!skipdesc)
    scr_pausedesc();
skipdesc=0;