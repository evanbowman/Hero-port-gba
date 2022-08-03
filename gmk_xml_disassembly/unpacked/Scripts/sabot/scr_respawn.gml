//RESPAWN
if (global.forever && newforeverlevel){
    newforeverlevel=0;
    with (obj_block)
        instance_destroy();
    scr_forevercreate();
    for (i=0;i<15;i+=1){
        for (j=0;j<15;j+=1){
            mroom[i,j]=0;
            mitem[i,j]=0;
            mitemclear[i,j]=0;
            mdoorclear[i,j]=0;
            mbarrierclear[i,j]=0;
            mtextclear[i,j]=0;
            }
        }
    }
if (global.bossmode){
    scr_bossmodestats();
    for (i=0;i<15;i+=1){
        for (j=0;j<15;j+=1){
            mroom[i,j]=0;
            mitem[i,j]=0;
            mitemclear[i,j]=0;
            mdoorclear[i,j]=0;
            mbarrierclear[i,j]=0;
            mtextclear[i,j]=0;
            }
        }
    mtextclear[5,6]=1;
    }

sound_stop_all();
x=respawnroomx*320;
y=respawnroomy*320;
scr_scrollprepare();
instance_deactivate_all(1);
scr_scroll();
if (instance_number(obj_savepoint_s)==1)
    instance_create(obj_savepoint_s.x+16,obj_savepoint_s.y-32,obj_hero);
else
    instance_create(x+160,y+160,obj_hero);
if (!alreadykilledtext && !global.bossmode && !global.forever && !global.reallyplay){
    alreadykilledtext=1;
    text1=scr_omnitext(512);
    text2=scr_omnitext(513);
    text3=scr_omnitext(514);
    }
pickuppresent=0;
if (global.forever && foreverlevel>1)
    global.hp+=0;
else{
    global.maxhp=global.level+10;
    global.hp=global.maxhp;
    }
flashblur=0;
