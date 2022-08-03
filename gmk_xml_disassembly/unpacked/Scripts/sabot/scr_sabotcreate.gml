scr_joycreate();
if (global.forever)
    scr_forevercreate();
cannotpause=1;
middletext=scr_omnitext(477);
if (room==rom_quest1 || room==rom_quest2)
    scr_hintorder();

extstring1=scr_omnitext(585);
extstring2=scr_omnitext(586);
extstring3=scr_omnitext(587);
extstring4=scr_omnitext(588);
extstring5=scr_omnitext(589);
extstring6=scr_omnitext(519);
extstring7=scr_omnitext(520);
extstring8=scr_omnitext(590);

//really
if (global.reallyplay){
    x=320;
    y=320;
    global.difficulty=2;
    }

//Map
for (i=0;i<15;i+=1){
    for (j=0;j<15;j+=1){
        mroom[i,j]=0; //rooms, connections
        mitem[i,j]=0; //save points(1), boss+item+computer(2)
        mitemclear[i,j]=0; //boss+item+computer cleared
        mdoorclear[i,j]=0; //doors cleared
        mbarrierclear[i,j]=0; //generator/barriers cleared
        mtextclear[i,j]=0; //oncetext cleared
        }
    }
//NEW
if (global.newgame){
    roomx=floor(x/320);
    roomy=floor(y/320);
    respawnroomx=roomx;
    respawnroomy=roomy;
    if (global.difficulty>0)
        global.hardmode=1;
    else
        global.hardmode=0;
    global.seconds=0;
    global.blaster=1;
    global.blade=0;
    global.suit=0;
    global.computers=0;
    global.expel=0;
    global.patternscan=0;
    global.level=0;
    global.maxhp=10+global.level;
    global.hp=global.maxhp;
    global.oncespecialtext="!";
    global.eliminator="!";
    global.extraspecial="!";
    if (global.difficulty==0 || global.difficulty==1)
        mitem[7,7]=2;
    if (!global.bossmode && !global.forever && !global.reallyplay){
        scr_checkfile();
        if (!wrongsum)
            scr_save();
        }
    }
//LOAD
else{
    scr_checkfile();
    if (!wrongsum){
        scr_load(global.gamefile);
        x=roomx*320;
        y=roomy*320;
        respawnroomx=roomx;
        respawnroomy=roomy;
        }
    //FILE WAS CORRUPT
    else{
        roomx=floor(x/320);
        roomy=floor(y/320);
        respawnroomx=roomx;
        respawnroomy=roomy;
        if (global.difficulty==0 || global.difficulty==1)
            mitem[7,7]=2;
        }
    }

gamesabot=1;
zone="";
zone2="";
zonenum=0;
threat=0;
prevzone="";
text1="";
text2="";
text3="";
prevtext1="";
prevtext2="";
prevtext3="";
oldtext1="";
oldtext2="";
oldtext3="";
prevzone="";
tier="";
textcyc=0;
yfu=0;
instance_create(x,y,obj_bgdraw);
blackinit=0;
for (i=0;i<5;i+=1)
    pickfps[i]=1;

//Deactivate surrounding world
instance_deactivate_all(1);
scr_scroll();
respawnroomx=roomx;
respawnroomy=roomy;
if (instance_number(obj_savepoint_s)==1)
    tempidh=instance_create(obj_savepoint_s.x+16,obj_savepoint_s.y,obj_hero);
else
    tempidh=instance_create(x+160,y+160,obj_hero);
if (global.reallyplay)
    tempidh.cannotwarp=1;
with (obj_savepoint)
    instance_destroy();

//BLACK CIRCLE
for (i=0;i<10;i+=1){
    for (j=0;j<10;j+=1)
        instance_create(x+i*32,y+j*32,obj_blackcircle);
    }
//Boss mode
global.autofire=0;
if (global.bossmode){
    scr_bossmodestats();
    if (global.normalrush)
        global.hardmode=0;
    }
if (global.forever){
    scr_foreverstats();
    }
