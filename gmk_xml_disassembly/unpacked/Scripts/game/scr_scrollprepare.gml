with (obj_deleteme)
    instance_destroy();
with (obj_lvl0)
    instance_destroy();
with (obj_blur)
    instance_destroy();
with (obj_dirt)
    instance_destroy();
with (obj_pipe1)
    instance_destroy();
with (obj_door1)
    instance_destroy();
with (obj_savepedestal)
    instance_destroy();
with (obj_block2){
    x=xstart;
    y=ystart;
    }
with (obj_black3)
    solid=1;
obj_sabot.alarm[2]=-1;
text1="";
text2="";
text3="";
obj_bgdraw.x=x;
obj_bgdraw.y=y;
shakescreen=0;
obj_sabot.pickuppresent=0;
boss=0;
with (obj_flower){
    ready=0;
    y=ystart;
    }