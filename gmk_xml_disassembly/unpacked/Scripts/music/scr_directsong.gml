if (global.music>0){
    jbfmod_stop_song();
    jbfmod_play_song(argument0);
    jbfmod_set_master_volume(global.music*64);
    global.currentsong=argument0;
    global.wantsong=argument0;
    global.fadesong=0;
    }