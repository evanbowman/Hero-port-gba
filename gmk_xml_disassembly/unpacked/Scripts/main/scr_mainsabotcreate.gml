scr_compat(0);
global.fadesong=0;
global.language=0;
global.languages=6; //+1

for (i=0;i<90;i+=1){
    tempx=2+random(386);
    tempy=2+random(342);
    go=1;
    if (tempx>48 && tempx<352 && tempy>8 && tempy<152)
        go=0;
    else if (tempx>106 && tempx<292 && tempy>178)
        go=0;
    if (go)
        instance_create(tempx,tempy,obj_star);
    }
scr_joycreate();
global.normalrush=0;
global.forever=0;
global.foreverbomb=0;
global.hypergun=0;
global.bossmode=0;
global.showhint=1;
global.flower=0;
global.handstand=0;
global.anniunlock=0;
global.bonusunlock=0;
global.really=0;
global.reallyplay=0;
global.annihilation=0;
global.bonus=0;
global.retromode=0;
global.newgame=0;
global.joyz=1;
global.joyx=2;
global.joypause=3;
global.joymap=4;
global.joyauto=5;
global.joyr=6;
global.joyu=7;
global.joyl=8;
global.joyd=9;

sheenmax=40+random(40);
gamesabot=0;
ignoreall=1;
text1="";
text2="";
text3="";
prevtext1="";
prevtext2="";
prevtext3="";
tier="main";
selected=1;
selections=1;
textcyc=0;
yx=64;
yx2=-16;
musictest=1;
global.fireleft=ord("Z");
global.fireright=ord("X");
global.sound=4;
global.music=4;
global.window=0; //1x, 2x, full
global.motionblur=1;
global.screenshaking=1;
global.heroflames=1;
global.damageflash=1;
global.besttime1=359999;
global.besttime2=359999;
global.besttime3=359999;
global.besttime4=359999;
global.besttime5=0;
global.besttime6=359999;
global.bestflags=0;
global.seconds=global.besttime1;
scr_timestring();
best1=timestring;
global.seconds=global.besttime2;
scr_timestring();
best2=timestring;
global.seconds=global.besttime3;
scr_timestring();
best3=timestring;
global.seconds=global.besttime4;
scr_timestring();
best4=timestring;
global.seconds=global.besttime6;
scr_timestring();
best6=timestring;


if (!file_exists("herocore.sav")){
    global.window=0;
    scr_compat(1);
    scr_newfile();
    }
else{
    scr_checkfile();
    if (!wrongsum){
        scr_loadkeys();
        global.seconds=global.besttime1;
        scr_timestring();
        best1=timestring;
        global.seconds=global.besttime2;
        scr_timestring();
        best2=timestring;
        global.seconds=global.besttime3;
        scr_timestring();
        best3=timestring;
        global.seconds=global.besttime4;
        scr_timestring();
        best4=timestring;
        global.seconds=global.besttime6;
        scr_timestring();
        best6=timestring;
        }
    }
scr_setlanguage();
scr_setmaps();
scr_volume();
scr_maintexts();
scr_maindesc();

for (i=1;i<11;i+=1){
    mdifficulty[i]=0;
    mtime[i]=0;
    mlevel[i]=0;
    mrooms[i]=0;
    mcomputers[i]=0;
    }
if (global.besttime1<359999)
    map1=1;
if (global.besttime2<359999)
    map2=1;
if (global.besttime3<359999){
    map3=1;
    showhpoption=1;
    global.havemap3=1;
    }
if (map1)
    map=1;
else if (map2)
    map=2;
else if (map3)
    map=3;
else
    map=1;