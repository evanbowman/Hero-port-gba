//argument0 1 is don't mute music
/*if (gamesabot){
    if (pause && tier="options"){
        scr_checkfile();
        if (!wrongsum)
            scr_savekeys();
        }
    }*/
if (global.forever){
    if (flags>global.bestflags)
        global.bestflags=flags;
    }
sound_stop_all();
if (argument0==0)
    jbfmod_stop_song();
scr_wantsong(0);
scr_checkfile();
if (wrongsum==0)
    scr_savekeys();
room_goto(rom_title);