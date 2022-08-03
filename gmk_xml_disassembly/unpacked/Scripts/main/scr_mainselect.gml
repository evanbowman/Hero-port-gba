if (!ignoreall){

//MAIN
if (tier="main"){
    if (selected==1){
        scr_checkfile();
        if (wrongsum>0){
            tier="loaderror";
            menubacksound=1;
            selected=1;
            }
        else{
            tier="start";
            scr_mainreadfile();
            scr_sound("menuselect");
            selected=2;
            }
        }
    else if (selected==2){
        tier="options";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==3){
        tier="gamepad";
        selected=1;
        scr_sound("menuselect");
        }
    else if (selected==4){
        if (global.language<global.languages)
            global.language+=1;
        else
            global.language=0;
        scr_sound("menuselect");
        scr_setlanguage();
        scr_maintexts();
        scr_maindesc();
        }
    else if (selected==5 && global.bonus){
        tier="bonus";
        selected=1;
        tempmusicvolume=global.music;
        if (global.music==0)
            global.music=1;
        scr_sound("menuselect");
        }
    else if (selected==5 && !global.bonus){
        scr_endgame();
        }
    else if (selected==6){
        scr_endgame();
        }
    }
//START
else if (tier="start"){
    global.gamefile=selected-2;
    if (selected==1){
        tier="main";
        menubacksound=1;
        }
    else if (selected>1 && selected<10){
        if (mdifficulty[selected-1]==0){
            scr_sound("menuselect");
            tier="difficulty";
            selected=2;
            }
        else{
            global.bossmode=0;
            scr_sound("menuselect");
            scr_wantsong(0);
            global.newgame=0;
            ignoreall=1;
            fadeblack=1;
            alarm[0]=25;
            global.difficulty=mdifficulty[selected-1]-1;
            scr_checkfile();
            if (!wrongsum)
                scr_savekeys();
            }
        }
    else if (selected==10){
        scr_checkfile();
        if (wrongsum>0){
            tier="loaderror";
            menubacksound=1;
            selected=1;
            }
        else{
            tier="copy";
            selected=1;
            scr_mainreadfile();
            scr_sound("menuselect");
            }
        }
    else if (selected==11){
        scr_checkfile();
        if (wrongsum>0){
            tier="loaderror";
            menubacksound=1;
            selected=1;
            }
        else{
            tier="delete";
            selected=1;
            scr_mainreadfile();
            scr_sound("menuselect");
            }
        }
    }
//DIFFICULTY
else if (tier="difficulty"){
    if (selected==1){
        tier="start";
        selected=global.gamefile+2;
        menubacksound=1;
        }
    //easy-annihilation
    else if (selected>1 && selected<5){
        global.newgame=1;
        global.bossmode=0;
        global.reallyplay=0;
        scr_wantsong(0);
        global.difficulty=selected-2;
        ignoreall=1;
        fadeblack=1;
        alarm[0]=25;
        scr_sound("menuselect");
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        }
    //really
    else if (selected==5){
        global.newgame=1;
        global.bossmode=0;
        global.reallyplay=1;
        scr_wantsong(0);
        global.difficulty=1;
        ignoreall=1;
        fadeblack=1;
        alarm[0]=25;
        scr_sound("menuselect");
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        }
    }
//LOAD ERROR
else if (tier="loaderror"){
    if (selected==1){
        tier="main";
        menubacksound=1;
        }
    else if (selected==2){
        scr_newfile();
        tier="start";
        selected=1;
        scr_sound("menuselect");
        scr_mainreadfile();
        }
    }
//OPTIONS
else if (tier="options"){
    if (selected==1){
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        tier="main";
        selected=2;
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
        jbfmod_set_master_volume(64*global.music);
        if (global.music==0){
            jbfmod_stop_song();
            global.fadesong=0;
            }
        else if (global.music==4)
            scr_wantsong(global.currentsong);
        scr_sound("menuselect");
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
//COPY
else if (tier=="copy"){
    if (selected==1){
        menubacksound=1;
        tier="start";
        selected=1;
        }
    else{
        scr_checkfile();
        if (wrongsum>0){
            tier="loaderror";
            menubacksound=1;
            selected=1;
            }
        else{
            tier="copyto";
            copytarget=selected-2;
            scr_sound("menuselect");
            }
        }
    }
//COPY TO
else if (tier=="copyto"){
    if (selected==1){
        menubacksound=1;
        tier="start";
        selected=1;
        }
    else{
        scr_checkfile();
        if (wrongsum>0){
            tier="loaderror";
            menubacksound=1;
            selected=1;
            }
        else{
            scr_copyfile(copytarget,selected-2);
            scr_mainreadfile();
            scr_sound("save");
            tier="start";
            }
        }
    }
//DELETE
else if (tier=="delete"){
    if (selected==1){
        menubacksound=1;
        tier="start";
        selected=1;
        }
    else{
        scr_checkfile();
        if (wrongsum>0){
            tier="loaderror";
            menubacksound=1;
            selected=1;
            }
        else{
            scr_deletefile(selected-2);
            scr_mainreadfile();
            scr_sound("explo2");
            tier="start";
            }
        }
    }
//GAMEPAD
else if (tier="gamepad"){
    if (selected==1){
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        tier="main";
        selected=3;
        scr_sound("menuselect");
        }
    }

//BONUS
else if (tier="bonus"){
    if (selected==1){
        tier="main";
        menubacksound=1;
        selected=5;
        global.music=tempmusicvolume;
        if (global.music==0)
            scr_wantsong(0);
        }
    else if (selected==2){
        global.music=tempmusicvolume;
        global.bossmode=1;
        scr_sound("menuselect");
        scr_wantsong(0);
        global.newgame=1;
        ignoreall=1;
        fadeblack=1;
        alarm[0]=25;
        global.difficulty=2;
        global.hardmode=0;
        global.normalrush=1;
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        }
    else if (selected==3){
        global.music=tempmusicvolume;
        global.bossmode=1;
        scr_sound("menuselect");
        scr_wantsong(0);
        global.newgame=1;
        ignoreall=1;
        fadeblack=1;
        alarm[0]=25;
        global.difficulty=2;
        global.hardmode=1;
        global.normalrush=0;
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        }
    else if (selected==4){
        global.music=tempmusicvolume;
        global.forever=1;
        scr_sound("menuselect");
        scr_wantsong(0);
        global.newgame=1;
        ignoreall=1;
        fadeblack=1;
        alarm[0]=25;
        global.difficulty=2;
        global.hardmode=0;
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        }
    else if (selected==5){
        if (musictest==16){
            scr_directsong(musictest);
            alarm[6]=80;
            }
        else{
            scr_wantsong(musictest);
            alarm[6]=0;
            }
        }
    else if (selected==6){
        tier="maps";
        showmap=1;
        selected=1;
        scr_sound("menuselect");
        with (obj_sheen)
            instance_destroy();
        nosheen=1;
        }
    else if (selected==8){
        if (global.showhp)
            global.showhp=0;
        else
            global.showhp=1;
        scr_sound("menuselect");
        }
    }
//MAPS
else if (tier="maps"){
    tier="bonus";
    selected=6;
    showmap=0;
    menubacksound=1;
    nosheen=0;
    }
    
scr_maintexts();
if (!skipdesc)
    scr_maindesc();
skipdesc=0;
}
//Skip intro
else if (blackinit<314 && blackinit>4)
    blackinit=314;