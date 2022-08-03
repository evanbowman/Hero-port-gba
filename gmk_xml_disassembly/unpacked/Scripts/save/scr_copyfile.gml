//copy from argument0 to argument1
if (file_exists("herocore.sav")){
    fileid=file_text_open_read("herocore.sav");
    for (i=0;i<18;i+=1){
        savestring[i]=file_text_read_string(fileid);
        file_text_readln(fileid);
        }
    file_text_close(fileid);

    savestring[argument1*2]=savestring[argument0*2];

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