//argument0 1 = generate a global string only
//argument0 2 = use the generated global string
//argument0 0 = save direct
if (global.blaster<=3 && global.blade<=3 && global.suit<=3 && global.level<=10 && global.computers<=10){
if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        savestring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    file_text_close(fileid);

    str=string(global.difficulty+1);
    if (global.seconds<100000) str+="0";
    if (global.seconds<10000) str+="0";
    if (global.seconds<1000) str+="0";
    if (global.seconds<100) str+="0";
    if (global.seconds<10) str+="0";
    str+=string(global.seconds);
    str+=chr(33+obj_sabot.respawnroomx);
    str+=chr(33+obj_sabot.respawnroomy);
    str+=string(global.blaster);
    str+=string(global.blade);
    str+=string(global.suit);
    str+=chr(33+global.computers);
    str+=string(global.shipscan*1+global.patternscan*2+global.expel*4);
    str+=chr(33+global.level);
    str+=global.oncespecialtext;
    str+=global.eliminator;
    str+=global.extraspecial;
    with (obj_sabot){
        str2="";
        for (a=0;a<15;a+=1){
            for (b=0;b<15;b+=1){
                str2+=chr(33+mroom[a,b]);
                str2+=chr(33+mitem[a,b]+4*mitemclear[a,b]+8*mdoorclear[a,b]+16*mbarrierclear[a,b]+32*mtextclear[a,b]);
                }
            }
        }
    
    if (argument0==1)
        global.endsavestring=str+obj_sabot.str2;
    else{
        if (argument0==2)
            savestring[global.gamefile*2]=global.endsavestring;
        else
            savestring[global.gamefile*2]=str+obj_sabot.str2;
    
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
    }
}