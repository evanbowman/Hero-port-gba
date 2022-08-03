if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        readstring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    workstring=readstring[16];

    global.fireleft=ord(string_char_at(workstring,1));
    global.fireright=ord(string_char_at(workstring,2));
    global.sound=real(string_char_at(workstring,3));
    global.music=real(string_char_at(workstring,4));
    global.window=real(string_char_at(workstring,5));
    scr_compat(1);
    global.motionblur=real(string_char_at(workstring,6));
    global.screenshaking=real(string_char_at(workstring,7));
    global.heroflames=real(string_char_at(workstring,8));
    global.damageflash=real(string_char_at(workstring,9));
    global.besttime1=real(string_copy(workstring,10,6));
    global.besttime2=real(string_copy(workstring,16,6));
    global.besttime3=real(string_copy(workstring,22,6));
    global.besttime4=real(string_copy(workstring,28,6));
    global.annihilation=real(string_char_at(workstring,34));
    global.retromode=real(string_char_at(workstring,35));
    global.joyz=ord(string_char_at(workstring,36))-48;
    global.joyx=ord(string_char_at(workstring,37))-48;
    global.joypause=ord(string_char_at(workstring,38))-48;
    global.joymap=ord(string_char_at(workstring,39))-48;
    global.joyr=ord(string_char_at(workstring,40))-48;
    global.joyu=ord(string_char_at(workstring,41))-48;
    global.joyl=ord(string_char_at(workstring,42))-48;
    global.joyd=ord(string_char_at(workstring,43))-48;
    global.bonus=real(string_char_at(workstring,44));
    global.joyauto=ord(string_char_at(workstring,45))-48;
    global.besttime5=real(string_copy(workstring,46,3));
    global.bestflags=real(string_copy(workstring,49,4));
    global.language=ord(string_char_at(workstring,53))-48;
    if (global.language>global.languages)
        global.language=0;
    global.showhint=real(string_char_at(workstring,54));
    global.besttime6=real(string_copy(workstring,55,6));
    
    if (global.besttime4==0)
        global.besttime4=359999;
    if (global.besttime6==0)
        global.besttime6=359999;

    file_text_close(fileid);
    }