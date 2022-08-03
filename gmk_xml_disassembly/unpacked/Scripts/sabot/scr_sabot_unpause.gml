pause=0;
if (changedoptions){
    scr_checkfile();
    if (!wrongsum)
        scr_savekeys();
    }
scr_setmaps();
menubacksound=1;
instance_activate_region(x,y,310,319,1);
instance_activate_object(obj_deleteme);
instance_activate_object(obj_blur);
text1=oldtext1;
text2=oldtext2;
text3=oldtext3;
textcyc=50;
goaroundtextcyc=1;
obj_hero.mapstopfire=2;
resetpickfps=1;