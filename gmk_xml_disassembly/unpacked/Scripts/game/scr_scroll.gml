scr_scrollprepare();

if (global.gmversion==5){
    view_left[0]=x-40;
    view_top[0]=y-40+shakescreen;
    }
else{
    view_xview[0]=x-40;
    view_yview[0]=y-40+shakescreen;
    }
roomx=floor(x/320);
roomy=floor(y/320);
instance_deactivate_region(x-320,y-320,959,959,1,1);
instance_activate_region(x,y,319,319,1);
if (instance_number(obj_loc)==1){
    if (obj_loc.zonenum!=0){
        zonenum=obj_loc.zonenum;
        if (zonenum==1) {zone=scr_omnitext(486); threat=1;}
        else if (zonenum==2) {zone=scr_omnitext(487); threat=2;}
        else if (zonenum==3) {zone=scr_omnitext(488); threat=3;}
        else if (zonenum==4) {zone=scr_omnitext(489); threat=4;}
        else if (zonenum==5) {zone=scr_omnitext(490); threat=5;}
        else if (zonenum==6) {zone=scr_omnitext(491); threat=6;}
        else if (zonenum==7) {zone=scr_omnitext(492); threat=7;}
        else if (zonenum==8) {zone=scr_omnitext(493); threat=8;}
        else if (zonenum==9) {zone=scr_omnitext(494); threat=9;}
        else if (zonenum==10) {zone=scr_omnitext(495); threat=10;}
        else if (zonenum==11) {zone=scr_omnitext(496); threat=0;}
        else if (zonenum==12) {zone=scr_omnitext(497); threat=3;}
        else if (zonenum==13) {zone=scr_omnitext(498); threat=5;}
        else if (zonenum==14) {zone=scr_omnitext(499); threat=10;}
        else if (zonenum==15) {zone=scr_omnitext(500); threat=10;}
        else if (zonenum==16) {zone=scr_omnitext(501); threat=10;}
        else if (zonenum==17) {zone=scr_omnitext(502); threat=10;}
        else if (zonenum==18) {zone=scr_omnitext(503); threat=15;}
        else if (zonenum==19) {zone=scr_omnitext(504); threat=0;}
        else if (zonenum==20) {zone=scr_omnitext(505); threat=10;}
        else if (zonenum==21) {zone=scr_omnitext(506); threat=0;}
        else if (zonenum==22) {zone=scr_omnitext(507); threat=10;}
        else if (zonenum==23) {zone=scr_omnitext(508);}
        else if (zonenum==24) {zone="really"; threat=255;} //hardcoded
        else if (zonenum==99) {zone="???"; threat=0;} //hardcoded

        if (obj_loc.threat>0)
            threat=obj_loc.threat;
        zone2=scr_omnitext(480)+" "+string(threat);
        if (zonenum==10 && global.oncespecialtext=="!"){
            text1=scr_omnitext(481);
            text2=scr_omnitext(482);
            text3=scr_omnitext(483);
            global.oncespecialtext="X";
            obj_hero.ignoreall=1;
            obj_hero.alarm[0]=40;
            }
        }
    //Pull text from loc
    if (mtextclear[roomx,roomy]==0 && obj_loc.text){
        if (obj_loc.text1>0)
            text1=scr_omnitext(obj_loc.text1);
        else
            text1="";
        if (obj_loc.text2>0)
            text2=scr_omnitext(obj_loc.text2);
        else
            text2="";
        if (obj_loc.text3>0)
            text3=scr_omnitext(obj_loc.text3);
        else
            text3="";
        //Forever special case
        if (global.forever && foreverlevel==1)
            text3=scr_omnitext(298)+string(global.besttime5)+"   "+scr_omnitext(299)+string(global.bestflags);
        //Clear text array
        mtextclear[roomx,roomy]=1;
        if (obj_loc.text && !global.forever){
            obj_hero.ignoreall=1;
            obj_hero.alarm[0]=30;
            }
        }
    }

//CALC ROOM
if (mroom[roomx,roomy]==0){
    scr_calcroom();
    }
with (obj_black3)
    solid=0;

//MULTIPLE SPAWNS
with (obj_dirtbig_s){
    if (place_free(x,y)) instance_create(x,y,obj_dirt);
    if (place_free(x+16,y)) instance_create(x+16,y,obj_dirt);
    if (place_free(x+32,y)) instance_create(x+32,y,obj_dirt);
    if (place_free(x,y+16)) instance_create(x,y+16,obj_dirt);
    if (place_free(x+16,y+16)) instance_create(x+16,y+16,obj_dirt);
    if (place_free(x+32,y+16)) instance_create(x+32,y+16,obj_dirt);
    if (place_free(x,y+32)) instance_create(x,y+32,obj_dirt);
    if (place_free(x+16,y+32)) instance_create(x+16,y+32,obj_dirt);
    if (place_free(x+32,y+32)) instance_create(x+32,y+32,obj_dirt);
    }
with (obj_pipe1long_s){
    if (place_free(x,y) || instance_number(obj_warmachine_s)==1) instance_create(x,y,obj_pipe1);
    if (place_free(x,y+16) || instance_number(obj_warmachine_s)==1) instance_create(x,y+16,obj_pipe1);
    if (place_free(x,y+32) || instance_number(obj_warmachine_s)==1) instance_create(x,y+32,obj_pipe1);
    if (place_free(x,y+48) || instance_number(obj_warmachine_s)==1) instance_create(x,y+48,obj_pipe1);
    }
with (obj_pipe2long_s){
    if (place_free(x,y)) instance_create(x,y,obj_pipe2);
    if (place_free(x+16,y)) instance_create(x+16,y,obj_pipe2);
    if (place_free(x+32,y)) instance_create(x+32,y,obj_pipe2);
    if (place_free(x+48,y)) instance_create(x+48,y,obj_pipe2);
    }
with (obj_liquid1long_s){
    instance_create(x,y,obj_liquid1);
    instance_create(x+16,y,obj_liquid1);
    instance_create(x+32,y,obj_liquid1);
    instance_create(x+48,y,obj_liquid1);
    }
with (obj_liquid2big_s){
    instance_create(x,y,obj_liquid2);
    instance_create(x+16,y,obj_liquid2);
    instance_create(x+32,y,obj_liquid2);
    instance_create(x,y+16,obj_liquid2);
    instance_create(x+16,y+16,obj_liquid2);
    instance_create(x+32,y+16,obj_liquid2);
    instance_create(x,y+32,obj_liquid2);
    instance_create(x+16,y+32,obj_liquid2);
    instance_create(x+32,y+32,obj_liquid2);
    }
with (obj_liquid3long_s){
    instance_create(x,y,obj_liquid3);
    instance_create(x,y+16,obj_liquid3);
    instance_create(x,y+32,obj_liquid3);
    instance_create(x,y+48,obj_liquid3);
    }

//SPECIAL SPAWN
if (mitemclear[roomx,roomy]==0){
    with (obj_itemspawner){ //items, computers, bosses
        if (!nospawn)
            instance_create(x,y,spawn);
        }
    }
if (mdoorclear[roomx,roomy]==0){
    with (obj_door1_s){
        instance_create(x,y,spawn);
        if (obj_sabot.text1=="" && obj_sabot.text2=="" && obj_sabot.text3=="" && !global.bossmode){
            obj_sabot.text1=scr_omnitext(484);
            obj_sabot.text2=scr_omnitext(485);
            obj_sabot.text3="";
            }
        }
    with (obj_battledoor_s)
        instance_create(x,y,spawn);
    }
if (mbarrierclear[roomx,roomy]==0){
    with (obj_generator_s)
        instance_create(x,y,spawn);
    with (obj_barrier_s)
        instance_create(x,y,spawn);
    }
else{
    with (obj_barrier_s){
        (instance_create(x,y,spawn)).dud=1;
        }
    }
with (obj_savepoint_s){
    if (!global.bossmode)
        instance_create(x,y-24,obj_savepoint);
    instance_create(x,y,obj_savepedestal);
    }
with (obj_cage_s)
    (instance_create(x,y,spawn)).dir=dir;
with (obj_eyespy_s)
    (instance_create(x,y,spawn)).dir=dir;
with (obj_cirepart_s){
    repeat(8)
        instance_create(x+2+random(58),y+2+random(12),spawn);
    }
with (obj_cirepart2_s){
    repeat(8)
        instance_create(x,y,spawn);
    }
with (obj_crawlpart_s){
    tempid=instance_create(x+8,y+8,spawn);
    with (tempid){
        move_contact_solid(90,256);
        dir=0;
        }
    tempid=instance_create(x+8,y+8,spawn);
    with (tempid){
        move_contact_solid(180,256);
        dir=3;
        }
    tempid=instance_create(x+8,y+8,spawn);
    with (tempid){
        move_contact_solid(270,256);
        dir=2;
        }
    tempid=instance_create(x+8,y+8,spawn);
    with (tempid){
        move_contact_solid(0,256);
        dir=1;
        }
    }
with (obj_flutter_s){
    repeat(5)
        instance_create(x+floor(random(16))*2,y+floor(random(16))*2,spawn);
    }
with (obj_terror_s){
    tempid=instance_create(x,y,spawn);
    tempid.facing=facing;
    tempid.hyper=hyper;
    }

//SINGLE SPAWNS
with (obj_spawner)
    instance_create(x,y,spawn);

//OTHER
with (obj_wallshooter)
    cyc=0;
firstscroll=1;
with (obj_fblock)
    visible=1;
scr_scrollmusic();
obj_flower.ready=1;
with (obj_weed){
    y=myy;
    hide=0;
    }
with (obj_weed2){
    y=myy;
    hide=0;
    }