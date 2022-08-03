//Is it correct?
//Sets wrongsum in caller
if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        readstring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    file_text_close(fileid);
    wrongsum=0;
    for (i=0;i<18;i+=2){
        scr_calcsum(readstring[i]);
        if (checksum!=real(readstring[i+1]))
            wrongsum=1;
        if (real(readstring[i+1])<1000)
            wrongsum=2;
        if (string_length(readstring[i+1])<5)
            wrongsum=3;
        if (string_length(readstring[i])!=468 && string_length(readstring[i])!=60)
            wrongsum=4;
        }
    }
else
    wrongsum=5;