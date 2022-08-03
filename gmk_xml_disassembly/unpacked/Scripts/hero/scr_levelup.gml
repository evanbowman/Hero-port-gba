if (global.level<10){
    global.level+=1;
    global.maxhp=10+global.level;
    global.hp=global.maxhp;
    obj_bgdraw.alpha=0.05;
    instance_create(x,y-12,obj_levelup);
    for (i=0;i<8;i+=1)
        (instance_create(x,y,obj_firework)).direction=i*45;
    }