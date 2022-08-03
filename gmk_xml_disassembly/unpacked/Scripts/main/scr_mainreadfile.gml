if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        readstring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    for (i=1;i<9;i+=1){
        workstring=readstring[i*2-2];
        mdifficulty[i]=real(string_char_at(workstring,1));
        mzero[i]=string_char_at(workstring,18);
        mshipscan[i]=real(string_char_at(workstring,14));
        if (mshipscan[i]>=4){
            mexpel[i]=1;
            mshipscan[i]-=4;
            }
        if (mshipscan[i]>=2)
            mshipscan[i]-=2;
        mtime[i]=real(string_copy(workstring,2,6));
        mlevel[i]=ord(string_char_at(workstring,15))-33;
        mrooms[i]=0;
        mcomputers[i]=ord(string_char_at(workstring,13))-33;
        mapstring=string_copy(workstring,19,450);
        for (a=0;a<15;a+=1){
            for (b=0;b<15;b+=1){
                if (ord(string_char_at(mapstring,1+a*30+b*2))-33>0)
                    mrooms[i]+=1;
                }
            }
        }
    for (i=1;i<9;i+=1){
        if (mdifficulty[i]==0)
            filetext[i]=scr_omnitext(582);
        else{
            filetext[i]=scr_omnitext(583)+string(i);
            if (mlevel[i]==10 || mdifficulty[i]==3)
                filetext[i]+="$";
            if (mcomputers[i]==10)
                filetext[i]+="$";
            if ((mdifficulty[i]==3 && mrooms[i]>=50) || (mdifficulty[i]<3 && mrooms[i]>=189))
                filetext[i]+="$";
            //if (mshipscan[i]==1)
            //    filetext[i]+="$";
            //if (mexpel[i]==1)
            //    filetext[i]+="$";
            }
        }
    file_text_close(fileid);
    }
