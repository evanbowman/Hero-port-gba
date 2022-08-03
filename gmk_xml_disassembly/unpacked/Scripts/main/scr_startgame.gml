scr_init();
scr_setmaps();
if (global.bossmode)
    room_goto(rom_quest4);
else if (global.forever)
    room_goto(rom_quest5);
else if (global.newgame){
    if (global.difficulty==0){
        global.destination=1;
        global.cut=1;
        room_goto(rom_cut);
        }
    else if (global.difficulty==1){
        global.hardmode=1;
        global.destination=2;
        global.cut=1;
        room_goto(rom_cut);
        }
    else if (global.difficulty==2){
        global.hardmode=1;
        global.cut=5;
        global.destination=3;
        room_goto(rom_cut);
        }
    }
else{
    if (global.difficulty==0)
        room_goto(rom_quest1);
    else if (global.difficulty==1){
        global.hardmode=1;
        room_goto(rom_quest2);
        }
    else if (global.difficulty==2){
        global.hardmode=1;
        room_goto(rom_quest3);
        }
    }