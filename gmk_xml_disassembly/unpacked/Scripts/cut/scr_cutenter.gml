if (firsttext==40 && string_length(text1)<textcyc && string_length(text2)<textcyc && string_length(text3)<textcyc){
    if (chat==chats){
        if (global.cut==4 && !donescroll)
            superscroll=1-superscroll;
        else{
            alarm[0]=2;
            drawblack=1;
            ignoreall=1;
            ignoreenter=1;
            }
        }
    else{
        text1=texts1[chat];
        text2=texts2[chat];
        text3=texts3[chat];
        chat+=1
        scr_cutpics();
        }
    }