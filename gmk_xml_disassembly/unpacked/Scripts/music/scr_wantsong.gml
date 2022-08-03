//Always use this or directsong
//Argument0 is only fade to nothing
if (argument0==0){
    global.fadesong=40;
    global.wantsong=0;
    }
else{
    global.wantsong=argument0;
    if ((global.currentsong!=global.wantsong && global.fadesong==0) || jbfmod_get_current_song()==0){
        if (jbfmod_get_current_song()==0){
            global.currentsong=global.wantsong;
            if (global.music>0)
                jbfmod_play_song(global.currentsong)
            jbfmod_set_master_volume(64*global.music);
            }
        else
            global.fadesong=40;
        }
    }