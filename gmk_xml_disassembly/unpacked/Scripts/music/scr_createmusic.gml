//Master music init
if (!global.musicinit){
    jbfmod_init(3,16);
    global.musicinit=1;
    
    if (file_exists("music/zone1.xm"))
        jbfmod_load_song(1,"music/zone1.xm");
    else if (file_exists("zone1.xm"))
        jbfmod_load_song(1,"zone1.xm");
    
    if (file_exists("music/zone2.xm"))
        jbfmod_load_song(2,"music/zone2.xm");
    else if (file_exists("zone2.xm"))
        jbfmod_load_song(2,"zone2.xm");
    
    if (file_exists("music/zone3.xm"))
        jbfmod_load_song(3,"music/zone3.xm");
    else if (file_exists("zone3.xm"))
        jbfmod_load_song(3,"zone3.xm");
    
    if (file_exists("music/zone4.xm"))
        jbfmod_load_song(4,"music/zone4.xm");
    else if (file_exists("zone4.xm"))
        jbfmod_load_song(4,"zone4.xm");
    
    if (file_exists("music/zone5.xm"))
        jbfmod_load_song(5,"music/zone5.xm");
    else if (file_exists("zone5.xm"))
        jbfmod_load_song(5,"zone5.xm");    
    
    if (file_exists("music/zone6.xm"))
        jbfmod_load_song(6,"music/zone6.xm");
    else if (file_exists("zone6.xm"))
        jbfmod_load_song(6,"zone6.xm");
    
    if (file_exists("music/zone7.xm"))
        jbfmod_load_song(7,"music/zone7.xm");
    else if (file_exists("zone7.xm"))
        jbfmod_load_song(7,"zone7.xm");
    
    if (file_exists("music/zone8.xm"))
        jbfmod_load_song(8,"music/zone8.xm");
    else if (file_exists("zone8.xm"))
        jbfmod_load_song(8,"zone8.xm");
    
    if (file_exists("music/zone9.xm"))
        jbfmod_load_song(9,"music/zone9.xm");
    else if (file_exists("zone9.xm"))
        jbfmod_load_song(9,"zone9.xm");
    
    if (file_exists("music/zone10.xm"))
        jbfmod_load_song(10,"music/zone10.xm");
    else if (file_exists("zone10.xm"))
        jbfmod_load_song(10,"zone10.xm");
    
    if (file_exists("music/title.xm"))
        jbfmod_load_song(11,"music/title.xm");
    else if (file_exists("title.xm"))
        jbfmod_load_song(11,"title.xm");
    
    if (file_exists("music/story.xm"))
        jbfmod_load_song(12,"music/story.xm");
    else if (file_exists("story.xm"))
        jbfmod_load_song(12,"story.xm");
    
    if (file_exists("music/boss.xm"))
        jbfmod_load_song(13,"music/boss.xm");
    else if (file_exists("boss.xm"))
        jbfmod_load_song(13,"boss.xm");
    
    if (file_exists("music/tetron.xm"))
        jbfmod_load_song(14,"music/tetron.xm");
    else if (file_exists("tetron.xm"))
        jbfmod_load_song(14,"tetron.xm");
    
    if (file_exists("music/ending.xm"))
        jbfmod_load_song(15,"music/ending.xm");
    else if (file_exists("ending.xm"))
        jbfmod_load_song(15,"ending.xm");
    
    if (file_exists("music/fanfare.xm"))
        jbfmod_load_song(16,"music/fanfare.xm");
    else if (file_exists("fanfare.xm"))
        jbfmod_load_song(16,"fanfare.xm");
    }