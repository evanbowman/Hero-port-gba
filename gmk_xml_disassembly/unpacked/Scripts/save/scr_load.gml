//Executed by Sabot ingame
if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        readstring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    workstring=readstring[global.gamefile*2];

    global.difficulty=real(string_char_at(workstring,1))-1;
    if (global.difficulty>0)
        global.hardmode=1;
    else
        global.hardmode=0;
    global.seconds=real(string_copy(workstring,2,6));
    roomx=ord(string_char_at(workstring,8))-33;
    roomy=ord(string_char_at(workstring,9))-33;
    global.blaster=real(string_char_at(workstring,10));
    global.blade=real(string_char_at(workstring,11));
    global.suit=real(string_char_at(workstring,12));
    global.computers=ord(string_char_at(workstring,13))-33;
    temp=real(string_char_at(workstring,14));
    if (temp>=4){
        temp-=4;
        global.expel=1;
        }
    if (temp>=2){
        temp-=2;
        global.patternscan=1;
        }
    if (temp==1)
        global.shipscan=1;
    global.level=ord(string_char_at(workstring,15))-33;
    global.maxhp=10+global.level;
    global.hp=global.maxhp;
    global.oncespecialtext=string_char_at(workstring,16);
    global.eliminator=string_char_at(workstring,17);
    global.extraspecial=string_char_at(workstring,18);

    mapstring=string_copy(workstring,19,450);
    for (a=0;a<15;a+=1){
        for (b=0;b<15;b+=1){
            mroom[a,b]=ord(string_char_at(mapstring,1+a*30+b*2))-33;
            temp=ord(string_char_at(mapstring,2+a*30+b*2))-33;
            if (temp>=32){
                temp-=32;
                mtextclear[a,b]=1;
                }
            if (temp>=16){
                temp-=16;
                mbarrierclear[a,b]=1;
                }
            if (temp>=8){
                temp-=8;
                mdoorclear[a,b]=1;
                }
            if (temp>=4){
                temp-=4;
                mitemclear[a,b]=1;
                }
            mitem[a,b]=temp;
            }
        }

    file_text_close(fileid);
    }