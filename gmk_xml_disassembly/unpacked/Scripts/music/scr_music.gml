//Fade now
if (global.fadesong>0){
    global.fadesong-=1;
    songvol=global.fadesong*global.music*1.6
    if (songvol>256)
        songvol=256;
    if (songvol<0)
        songvol=0;
    jbfmod_set_master_volume(songvol);
    //Play new song
    if (global.fadesong==0){
        global.currentsong=global.wantsong;
        if (global.currentsong>0){
            if (global.music>0)
                jbfmod_play_song(global.currentsong)
            }
        else
            jbfmod_stop_song();
        jbfmod_set_master_volume(64*global.music);
        }
    }