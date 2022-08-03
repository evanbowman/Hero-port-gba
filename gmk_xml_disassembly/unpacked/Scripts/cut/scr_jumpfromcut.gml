if (global.destination==1)
    room_goto(rom_quest1);
else if (global.destination==2)
    room_goto(rom_quest2);
else if (global.destination==3)
    room_goto(rom_quest3);
else if (global.destination==4){
    global.destination=5;
    global.cut=4;
    room_goto(rom_cut);
    }
else if (global.destination==5){
    room_goto(rom_endsave);
    scr_wantsong(0);
    }