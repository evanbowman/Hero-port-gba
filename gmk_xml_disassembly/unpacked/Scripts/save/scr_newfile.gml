fileid=file_text_open_write("herocore.sav");
for (i=0;i<8;i+=1){
    writestring="0000000!!000!0!!!!";
    for (j=0;j<9;j+=1)
        writestring+="!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    file_text_write_string(fileid,writestring);
    file_text_writeln(fileid);
    scr_calcsum(writestring);
    file_text_write_string(fileid,string(checksum));
    file_text_writeln(fileid);
    }
writestring="ZX4411111359999359999359999359999001234567800000000001000000";
file_text_write_string(fileid,writestring);
file_text_writeln(fileid);
scr_calcsum(writestring);
file_text_write_string(fileid,string(checksum));
file_text_writeln(fileid);
file_text_writeln(fileid);
scr_savefilerap();
file_text_close(fileid);
