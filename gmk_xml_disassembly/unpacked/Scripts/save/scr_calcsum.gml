//argument0 is string to calc
len=string_length(argument0);
checksum=0;
for (check=1;check<len+1;check+=1){
    checksum+=(ord(string_char_at(argument0,check))-32)*check;
    }
repeat(3){
    if (string_char_at(string(checksum),3)=="0") checksum+=189;
    else if (string_char_at(string(checksum),6)=="1") checksum+=853;
    else if (string_char_at(string(checksum),6)=="2") checksum+=234;
    else if (string_char_at(string(checksum),6)=="3") checksum+=780;
    else if (string_char_at(string(checksum),6)=="4") checksum+=643;
    else if (string_char_at(string(checksum),6)=="5") checksum+=347;
    else if (string_char_at(string(checksum),6)=="6") checksum+=884;
    else if (string_char_at(string(checksum),6)=="7") checksum+=132;
    else if (string_char_at(string(checksum),6)=="8") checksum+=884;
    else if (string_char_at(string(checksum),6)=="9") checksum+=213;
    }
if (string_char_at(string(checksum),3)=="0") checksum*=12;
else if (string_char_at(string(checksum),3)=="1") checksum*=7;
else if (string_char_at(string(checksum),3)=="2") checksum*=9;
else if (string_char_at(string(checksum),3)=="3") checksum*=3;
else if (string_char_at(string(checksum),3)=="4") checksum*=4;
else if (string_char_at(string(checksum),3)=="5") checksum*=6;
else if (string_char_at(string(checksum),3)=="6") checksum*=8;
else if (string_char_at(string(checksum),3)=="7") checksum*=2;
else if (string_char_at(string(checksum),3)=="8") checksum*=5;
else if (string_char_at(string(checksum),3)=="9") checksum*=11;
repeat(3){
    if (string_char_at(string(checksum),6)=="0") checksum+=9638;
    else if (string_char_at(string(checksum),6)=="1") checksum+=236;
    else if (string_char_at(string(checksum),6)=="2") checksum+=8906;
    else if (string_char_at(string(checksum),6)=="3") checksum+=362;
    else if (string_char_at(string(checksum),6)=="4") checksum+=3085;
    else if (string_char_at(string(checksum),6)=="5") checksum+=7834;
    else if (string_char_at(string(checksum),6)=="6") checksum+=609;
    else if (string_char_at(string(checksum),6)=="7") checksum+=366;
    else if (string_char_at(string(checksum),6)=="8") checksum+=7743;
    else if (string_char_at(string(checksum),6)=="9") checksum+=1233;
    }
checksum*=1+real(string_char_at(string(checksum),5));
checksum+=1;