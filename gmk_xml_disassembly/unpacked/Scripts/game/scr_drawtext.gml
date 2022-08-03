//scr_drawtext(x,y,string,sep,w,sprite,firstchar,scale)
//original by Captain Goodnight, optimized for Hero Core by DCR

drstr=argument2;
drstrcheck="";
drstrcount=0;
for (drti=0; drti<string_length(drstr); drti+=1){
    drstrcheck=string_char_at(drstr,drti+1)
    draw_sprite(argument5,ord(drstrcheck)-argument6,argument0+(8*drstrcount),argument1);
    drstrcount+=1;
    }
