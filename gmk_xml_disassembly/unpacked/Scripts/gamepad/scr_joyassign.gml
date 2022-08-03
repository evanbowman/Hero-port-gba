if (joystick_exists(1)){
//Catch only one button
temp=0;
for (i=1;i<imaxx+1;i+=1){
    if (joystick_check_button(1,i)){
        temp+=1;
        tempbutton=i;
        }
    }
//Assign
if (temp==1){
    if (selected==2){
        if (global.joyz!=tempbutton){
            global.joyz=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==3){
        if (global.joyx!=tempbutton){
            global.joyx=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==4){
        if (global.joypause!=tempbutton){
            global.joypause=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==5){
        if (global.joymap!=tempbutton){
            global.joymap=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==6){
        if (global.joyauto!=tempbutton){
            global.joyauto=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==7){
        if (global.joyr!=tempbutton){
            global.joyr=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==8){
        if (global.joyu!=tempbutton){
            global.joyu=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==9){
        if (global.joyl!=tempbutton){
            global.joyl=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    if (selected==10){
        if (global.joyd!=tempbutton){
            global.joyd=tempbutton;
            scr_maintexts();
            scr_sound("menuselect");
            }
        }
    }
}