for (i=0;i<13;i+=1)
    menutext[i]="";
//MAIN
if (tier=="main"){
    menutext[0]=scr_omnitext(356);
    menutext[1]="";
    menutext[2]=scr_omnitext(357);
    menutext[3]=scr_omnitext(358);
    menutext[4]=scr_omnitext(359);
    menutext[5]=scr_omnitext(310);
    menutext[6]=scr_omnitext(311);
    menutext[7]=scr_omnitext(360);
    menutext[8]=scr_omnitext(361);
    menutext[9]=scr_omnitext(362);
    selections=8;
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
//INFORMATION
else if (tier=="information"){
    menutext[0]=scr_omnitext(363);
    menutext[1]="";
    menutext[2]=scr_omnitext(315);
    menutext[3]=scr_omnitext(364);
    menutext[4]=scr_omnitext(365);
    menutext[5]=scr_omnitext(366);
    menutext[6]=scr_omnitext(367)+string(global.blaster);
    menutext[7]=scr_omnitext(368)+string(global.blade);
    menutext[8]=scr_omnitext(369)+string(global.suit);
    if (global.computers>0){
        if (room==rom_quest3)
            menutext[9]=scr_omnitext(370);
        else
            menutext[9]=scr_omnitext(371);
        }
    else
        menutext[9]="?";
    if (global.shipscan)
        menutext[10]=scr_omnitext(223);
    else
        menutext[10]="?";
    if (global.patternscan)
        menutext[11]=scr_omnitext(227);
    else
        menutext[11]="?";
    if (global.expel)
        menutext[12]=scr_omnitext(231);
    else
        menutext[12]="?";
    selections=11;
    }
//SAVE FIRST
else if (tier=="savefirst"){
    menutext[0]=scr_omnitext(374);
    menutext[1]="";
    menutext[2]=scr_omnitext(372);
    menutext[3]=scr_omnitext(373);
    selections=2;
    }
//QUIT TO TITLE
else if (tier=="quittitle"){
    menutext[0]=scr_omnitext(375);
    menutext[1]="";
    menutext[2]=scr_omnitext(372);
    menutext[3]=scr_omnitext(373);
    selections=2;
    }
//QUIT GAME
else if (tier=="quitgame"){
    menutext[0]=scr_omnitext(376);
    menutext[1]="";
    menutext[2]=scr_omnitext(372);
    menutext[3]=scr_omnitext(373);
    selections=2;
    }
//MAP
else if (tier=="map"){
    selections=1;
    }
//WARP
else if (tier=="warp"){
    selections=1;
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
//BOSSMODE
else if (tier=="bossmode"){
    if (global.forever)
        menutext[0]=scr_omnitext(377);
    else
        menutext[0]=scr_omnitext(378);
    menutext[1]="";
    menutext[2]=scr_omnitext(357);
    menutext[3]=scr_omnitext(379);
    menutext[4]=scr_omnitext(358);
    menutext[5]=scr_omnitext(310);
    menutext[6]=scr_omnitext(311);
    menutext[7]=scr_omnitext(361);
    menutext[8]=scr_omnitext(362);
    selections=7;
    }
//RETRY
else if (tier=="retry"){
    if (global.forever)
        menutext[0]=scr_omnitext(380);
    else
        menutext[0]=scr_omnitext(381);
    menutext[1]="";
    menutext[2]=scr_omnitext(372);
    menutext[3]=scr_omnitext(373);
    selections=2;
    }