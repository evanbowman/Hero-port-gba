if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        savestring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    file_text_close(fileid);

    str=scr_button(0)+scr_button(1);
    str+=string(global.sound)+string(global.music);
    str+=string(global.window)+string(global.motionblur)+string(global.screenshaking)+string(global.heroflames)+string(global.damageflash);
    if (global.besttime1<100000) str+="0";
    if (global.besttime1<10000) str+="0";
    if (global.besttime1<1000) str+="0";
    if (global.besttime1<100) str+="0";
    if (global.besttime1<10) str+="0";
    str+=string(global.besttime1);
    if (global.besttime2<100000) str+="0";
    if (global.besttime2<10000) str+="0";
    if (global.besttime2<1000) str+="0";
    if (global.besttime2<100) str+="0";
    if (global.besttime2<10) str+="0";
    str+=string(global.besttime2);
    if (global.besttime3<100000) str+="0";
    if (global.besttime3<10000) str+="0";
    if (global.besttime3<1000) str+="0";
    if (global.besttime3<100) str+="0";
    if (global.besttime3<10) str+="0";
    str+=string(global.besttime3);
    if (global.besttime4<100000) str+="0";
    if (global.besttime4<10000) str+="0";
    if (global.besttime4<1000) str+="0";
    if (global.besttime4<100) str+="0";
    if (global.besttime4<10) str+="0";
    str+=string(global.besttime4);
    str+=string(global.annihilation);
    str+=string(global.retromode);
    str+=chr(48+global.joyz);
    str+=chr(48+global.joyx);
    str+=chr(48+global.joypause);
    str+=chr(48+global.joymap);
    str+=chr(48+global.joyr);
    str+=chr(48+global.joyu);
    str+=chr(48+global.joyl);
    str+=chr(48+global.joyd);
    str+=string(global.bonus);
    str+=chr(48+global.joyauto);
    if (global.besttime5<100) str+="0";
    if (global.besttime5<10) str+="0";
    str+=string(global.besttime5);
    if (global.bestflags<1000) str+="0";
    if (global.bestflags<100) str+="0";
    if (global.bestflags<10) str+="0";
    str+=string(global.bestflags);
    str+=chr(48+global.language);
    str+=string(global.showhint);
    if (global.besttime6<100000) str+="0";
    if (global.besttime6<10000) str+="0";
    if (global.besttime6<1000) str+="0";
    if (global.besttime6<100) str+="0";
    if (global.besttime6<10) str+="0";
    str+=string(global.besttime6);
    savestring[16]=str;

    fileid=file_text_open_write("herocore.sav");
    for (i=0;i<18;i+=2){
        file_text_write_string(fileid,savestring[i]);
        file_text_writeln(fileid);
        scr_calcsum(savestring[i]);
        file_text_write_string(fileid,string(checksum));
        file_text_writeln(fileid);
        }
    file_text_writeln(fileid);
    scr_savefilerap();
    file_text_close(fileid);
    }