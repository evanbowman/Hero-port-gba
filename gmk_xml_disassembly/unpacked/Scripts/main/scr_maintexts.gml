for (i=0;i<13;i+=1)
    menutext[i]="";
//MAIN
if (tier=="main"){
    menutext[0]=scr_omnitext(308);
    menutext[1]="";
    menutext[2]=scr_omnitext(309);
    menutext[3]=scr_omnitext(310);
    menutext[4]=scr_omnitext(311);
    menutext[5]=scr_omnitext(0);
    menutext[6]=scr_omnitext(312);
    selections=5;
    if (global.bonus){
        menutext[6]=scr_omnitext(313);
        menutext[7]=scr_omnitext(312);
        selections=6;
        }
    }
//START
else if (tier=="start"){
    menutext[0]=scr_omnitext(314);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=filetext[1];
    menutext[4]=filetext[2];
    menutext[5]=filetext[3];
    menutext[6]=filetext[4];
    menutext[7]=filetext[5];
    menutext[8]=filetext[6];
    menutext[9]=filetext[7];
    menutext[10]=filetext[8];
    menutext[11]=scr_omnitext(316);
    menutext[12]=scr_omnitext(317);
    selections=11;
    }
//DIFFICULTY
else if (tier=="difficulty"){
    menutext[0]=scr_omnitext(318);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=scr_omnitext(319);
    menutext[4]=scr_omnitext(320);
    if (global.annihilation){
        menutext[5]=scr_omnitext(321);
        if (global.really){
            menutext[6]=scr_omnitext(290);
            selections=5;
            }
        else
            selections=4;
        }
    else
        selections=3;
    }
//LOAD ERROR
else if (tier=="loaderror"){
    menutext[0]=scr_omnitext(322);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=scr_omnitext(323);
    selections=2;
    }
//OPTIONS
else if (tier=="options"){
    menutext[0]=scr_omnitext(324);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=scr_omnitext(325)+string_lower(scr_button(0));
    menutext[4]=scr_omnitext(326)+string_lower(scr_button(1));
    menutext[5]=scr_omnitext(329);
    if (global.sound==0)
        menutext[5]+=scr_omnitext(327);
    else if (global.sound==1)
        menutext[5]+="25%";
    else if (global.sound==2)
        menutext[5]+="50%";
    else if (global.sound==3)
        menutext[5]+="75%";
    else if (global.sound==4)
        menutext[5]+="100%";
    menutext[6]=scr_omnitext(330);
    if (global.music==0)
        menutext[6]+=scr_omnitext(327);
    else if (global.music==1)
        menutext[6]+="25%";
    else if (global.music==2)
        menutext[6]+="50%";
    else if (global.music==3)
        menutext[6]+="75%";
    else if (global.music==4)
        menutext[6]+="100%";
    menutext[7]=scr_omnitext(331);
    if (global.window==0)
        menutext[7]+=scr_omnitext(327)+", 1x";
    else if (global.window==1)
        menutext[7]+=scr_omnitext(327)+", 2x";
    else if (global.window==2)
        menutext[7]+=scr_omnitext(328);
    menutext[8]=scr_omnitext(332);
    if (global.motionblur)
        menutext[8]+=scr_omnitext(328);
    else
        menutext[8]+=scr_omnitext(327);
    menutext[9]=scr_omnitext(333);
    if (global.screenshaking)
        menutext[9]+=scr_omnitext(328);
    else
        menutext[9]+=scr_omnitext(327);
    menutext[10]=scr_omnitext(334);
    if (global.heroflames)
        menutext[10]+=scr_omnitext(328);
    else
        menutext[10]+=scr_omnitext(327);
    menutext[11]=scr_omnitext(335);
    if (global.retromode)
        menutext[11]+=scr_omnitext(328);
    else
        menutext[11]+=scr_omnitext(327);
    menutext[12]=scr_omnitext(132);
    if (global.showhint)
        menutext[12]+=scr_omnitext(328);
    else
        menutext[12]+=scr_omnitext(327);
    selections=11;
    }
//COPY
else if (tier=="copy"){
    menutext[0]=scr_omnitext(336);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=filetext[1];
    menutext[4]=filetext[2];
    menutext[5]=filetext[3];
    menutext[6]=filetext[4];
    menutext[7]=filetext[5];
    menutext[8]=filetext[6];
    menutext[9]=filetext[7];
    menutext[10]=filetext[8];
    selections=9;
    }
//COPY TO
else if (tier=="copyto"){
    menutext[0]=scr_omnitext(337);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=filetext[1];
    menutext[4]=filetext[2];
    menutext[5]=filetext[3];
    menutext[6]=filetext[4];
    menutext[7]=filetext[5];
    menutext[8]=filetext[6];
    menutext[9]=filetext[7];
    menutext[10]=filetext[8];
    selections=9;
    }
//DELETE
else if (tier=="delete"){
    menutext[0]=scr_omnitext(338);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=filetext[1];
    menutext[4]=filetext[2];
    menutext[5]=filetext[3];
    menutext[6]=filetext[4];
    menutext[7]=filetext[5];
    menutext[8]=filetext[6];
    menutext[9]=filetext[7];
    menutext[10]=filetext[8];
    selections=9;
    }
//GAMEPAD
else if (tier=="gamepad"){
    menutext[0]=scr_omnitext(339);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=scr_omnitext(340)+string_lower(scr_button(0))+"\: "+string(global.joyz);
    menutext[4]=scr_omnitext(341)+string_lower(scr_button(1))+"\: "+string(global.joyx);
    menutext[5]=scr_omnitext(342)+string(global.joypause);
    menutext[6]=scr_omnitext(343)+string(global.joymap);
    menutext[7]=scr_omnitext(344)+string(global.joyauto);
    menutext[8]=scr_omnitext(345)+string(global.joyr);
    menutext[9]=scr_omnitext(346)+string(global.joyu);
    menutext[10]=scr_omnitext(347)+string(global.joyl);
    menutext[11]=scr_omnitext(348)+string(global.joyd);
    menutext[12]=scr_omnitext(349);
    selections=11;
    }
//BONUS
else if (tier=="bonus"){
    menutext[0]=scr_omnitext(350);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=scr_omnitext(351)+" _"+scr_omnitext(319)+"\";
    menutext[4]=scr_omnitext(351)+" _"+scr_omnitext(320)+"\";
    menutext[5]=scr_omnitext(352);
    menutext[6]=scr_omnitext(353);
    if (musictest==1) menutext[6]+="zone1";
    else if (musictest==2) menutext[6]+="zone2";
    else if (musictest==3) menutext[6]+="zone3";
    else if (musictest==4) menutext[6]+="zone4";
    else if (musictest==5) menutext[6]+="zone5";
    else if (musictest==6) menutext[6]+="zone6";
    else if (musictest==7) menutext[6]+="zone7";
    else if (musictest==8) menutext[6]+="zone8";
    else if (musictest==9) menutext[6]+="zone9";
    else if (musictest==10) menutext[6]+="zone10";
    else if (musictest==11) menutext[6]+="title";
    else if (musictest==12) menutext[6]+="story";
    else if (musictest==13) menutext[6]+="boss";
    else if (musictest==14) menutext[6]+="tetron";
    else if (musictest==15) menutext[6]+="ending";
    else if (musictest==16) menutext[6]+="fanfare";
    menutext[6]+=">";
    menutext[7]=scr_omnitext(354);
    if (map==1) menutext[7]+=scr_omnitext(319);
    else if (map==2) menutext[7]+=scr_omnitext(320);
    else if (map==3) menutext[7]+=scr_omnitext(321);
    menutext[7]+=">";
    menutext[8]=scr_omnitext(355);
    if (showhpoption){
        menutext[9]=scr_omnitext(576);
        if (global.showhp)
            menutext[9]+=scr_omnitext(328);
        else
            menutext[9]+=scr_omnitext(327);
        }
    selections=7;
    if (showhpoption)
        selections=8;
    }
//MAPS
else if (tier=="maps"){
    selections=1;
    }