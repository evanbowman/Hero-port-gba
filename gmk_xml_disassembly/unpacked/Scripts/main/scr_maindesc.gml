//MAIN
if (tier=="main"){
    text1=scr_omnitext(267);
    text2="";
    text3="";
    if (selected==4){
        text1=scr_omnitext(160);
        text2=scr_omnitext(530);
        text3="";
        }
    }
//START
else if (tier=="start"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected>1 && selected<10){
        if (mdifficulty[selected-1]==0){
            text1=scr_omnitext(268);
            text2="";
            text3="";
            }
        else{
            text1=scr_omnitext(124);
            if (mdifficulty[selected-1]==1) text1+=scr_omnitext(126);
            else if (mdifficulty[selected-1]==2) text1+=scr_omnitext(127);
            else if (mdifficulty[selected-1]==3) text1+=scr_omnitext(128);
            global.seconds=mtime[selected-1];
            scr_timestring();
            text1+=timestring;
            text2=scr_omnitext(130);
            if (mlevel[selected-1]<10) text2+=string(mlevel[selected-1]);
            else text2+="x";
            text2+=scr_omnitext(131)+string(mrooms[selected-1]);
            if (mcomputers[selected-1]>0){
                if (mdifficulty[selected-1]==3)
                    text3=scr_omnitext(269)+string(mcomputers[selected-1])+"/10";
                else
                    text3=scr_omnitext(270)+string(mcomputers[selected-1])+"/10";
                }
            else
                text3="";
            }
        }
    else if (selected==10){
        text1=scr_omnitext(271);
        text2="";
        text3="";
        }
    else if (selected==11){
        text1=scr_omnitext(272);
        text2="";
        text3="";
        }
    }
//DIFFICULTY
else if (tier=="difficulty"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected==2){
        text1=scr_omnitext(274);
        text2=scr_omnitext(275);
        text3=scr_omnitext(273)+best1;
        }
    else if (selected==3){
        text1=scr_omnitext(276);
        text2=scr_omnitext(277);
        text3=scr_omnitext(273)+best2;
        }
    else if (selected==4){
        text1=scr_omnitext(278);
        text2=scr_omnitext(279);
        text3=scr_omnitext(273)+best3;
        }
    else if (selected==5){
        text1=scr_omnitext(280);
        text2=scr_omnitext(281);
        text3=scr_omnitext(273)+"00:02.04 _"+scr_omnitext(282)+"\";
        }
    }
//LOAD ERROR
else if (tier=="loaderror"){
    text1=scr_omnitext(283);
    text2=scr_omnitext(284);
    text3="";
    }
//OPTIONS
else if (tier=="options"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected==2){
        text1=scr_omnitext(147);
        text2="";
        text3="";
        }
    else if (selected==3){
        text1=scr_omnitext(147);
        text2="";
        text3="";
        }
    else if (selected==4){
        text1=scr_omnitext(148);
        text2="";
        text3="";
        }
    else if (selected==5){
        text1=scr_omnitext(149);
        text2="";
        text3="";
        }
    else if (selected==6){
        text1=scr_omnitext(150);
        text2=scr_omnitext(151);
        text3=scr_omnitext(152);
        }
    else if (selected==7){
        text1=scr_omnitext(153);
        text2=scr_omnitext(154);
        text3=scr_omnitext(155);
        }
    else if (selected==8){
        text1=scr_omnitext(156);
        text2="";
        text3="";
        }
    else if (selected==9){
        text1=scr_omnitext(157);
        text2="";
        text3="";
        }
    else if (selected==10){
        text1=scr_omnitext(158);
        text2=scr_omnitext(159);
        text3="";
        }
    else if (selected==11){
        text1=scr_omnitext(174);
        text2=scr_omnitext(175);
        text3=scr_omnitext(176);
        }
    }
//COPY
else if (tier=="copy"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected>1){
        if (mdifficulty[selected-1]==0){
            text1=scr_omnitext(285);
            text2="";
            text3="";
            }
        else{
            text1=scr_omnitext(124);
            if (mdifficulty[selected-1]==1) text1+=scr_omnitext(126);
            else if (mdifficulty[selected-1]==2) text1+=scr_omnitext(127);
            else if (mdifficulty[selected-1]==3) text1+=scr_omnitext(128);
            global.seconds=mtime[selected-1];
            scr_timestring();
            text1+=timestring;
            text2=scr_omnitext(130);
            if (mlevel[selected-1]<10) text2+=string(mlevel[selected-1]);
            else text2+="x";
            text2+=scr_omnitext(131)+string(mrooms[selected-1]);
            if (mcomputers[selected-1]>0)
                text3=scr_omnitext(270)+string(mcomputers[selected-1])+"/10";
            else
                text3="";
            }
        }
    }
//COPY TO
else if (tier=="copyto"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected>1){
        if (mdifficulty[selected-1]==0){
            text1=scr_omnitext(286);
            text2="";
            text3="";
            }
        else{
            text1=scr_omnitext(124);
            if (mdifficulty[selected-1]==1) text1+=scr_omnitext(126);
            else if (mdifficulty[selected-1]==2) text1+=scr_omnitext(127);
            else if (mdifficulty[selected-1]==3) text1+=scr_omnitext(128);
            global.seconds=mtime[selected-1];
            scr_timestring();
            text1+=timestring;
            text2=scr_omnitext(130);
            if (mlevel[selected-1]<10) text2+=string(mlevel[selected-1]);
            else text2+="x";
            text2+=scr_omnitext(131)+string(mrooms[selected-1]);
            if (mcomputers[selected-1]>0)
                text3=scr_omnitext(270)+string(mcomputers[selected-1])+"/10";
            else
                text3="";
            }
        }
    }
//DELETE
else if (tier=="delete"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected>1){
        if (mdifficulty[selected-1]==0){
            text1=scr_omnitext(287);
            text2="";
            text3="";
            }
        else{
            text1=scr_omnitext(124);
            if (mdifficulty[selected-1]==1) text1+=scr_omnitext(126);
            else if (mdifficulty[selected-1]==2) text1+=scr_omnitext(127);
            else if (mdifficulty[selected-1]==3) text1+=scr_omnitext(128);
            global.seconds=mtime[selected-1];
            scr_timestring();
            text1+=timestring;
            text2=scr_omnitext(130);
            if (mlevel[selected-1]<10) text2+=string(mlevel[selected-1]);
            else text2+="x";
            text2+=scr_omnitext(131)+string(mrooms[selected-1]);
            if (mcomputers[selected-1]>0)
                text3=scr_omnitext(270)+string(mcomputers[selected-1])+"/10";
            else
                text3="";
            }
        }
    }
//GAMEPAD
else if (tier=="gamepad"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected>=2 && selected<=6){
        text1=scr_omnitext(248);
        text2=scr_omnitext(249);
        text3="";
        }
    else if (selected>=7 && selected<=10){
        text1=scr_omnitext(250);
        text2=scr_omnitext(251);
        text3=scr_omnitext(252);
        }
    else if (selected==11){
        text1=scr_omnitext(253);
        text2=scr_omnitext(254);
        text3=scr_omnitext(255);
        }
    }
//BONUS
else if (tier=="bonus"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected==2){
        //text1=scr_omnitext(288);
        //text2=scr_omnitext(289);
        if (global.besttime6<3*60) ranktext=scr_omnitext(290);
        else if (global.besttime6<4*60) ranktext=scr_omnitext(291);
        else if (global.besttime6<5*60) ranktext=scr_omnitext(292);
        else if (global.besttime6<6*60) ranktext=scr_omnitext(293);
        else if (global.besttime6<8*60) ranktext=scr_omnitext(294);
        else if (global.besttime6<10*60) ranktext="a";
        else if (global.besttime6<12*60) ranktext="b";
        else if (global.besttime6<14*60) ranktext="c";
        else if (global.besttime6<16*60) ranktext="d";
        else if (global.besttime6<18*60) ranktext="e";
        else ranktext="f";
        text1=scr_omnitext(273)+best6+scr_omnitext(295)+ranktext+"\";
        text2="";
        text3="";
        }
    else if (selected==3){
        //text1=scr_omnitext(288);
        //text2=scr_omnitext(289);
        if (global.besttime4<5*60) ranktext=scr_omnitext(290);
        else if (global.besttime4<6*60) ranktext=scr_omnitext(291);
        else if (global.besttime4<7*60) ranktext=scr_omnitext(292);
        else if (global.besttime4<8*60) ranktext=scr_omnitext(293);
        else if (global.besttime4<10*60) ranktext=scr_omnitext(294);
        else if (global.besttime4<12*60) ranktext="a";
        else if (global.besttime4<14*60) ranktext="b";
        else if (global.besttime4<16*60) ranktext="c";
        else if (global.besttime4<18*60) ranktext="d";
        else if (global.besttime4<20*60) ranktext="e";
        else ranktext="f";
        text1=scr_omnitext(273)+best4+scr_omnitext(295)+ranktext+"\";
        text2="";
        text3="";
        }
    else if (selected==4){
        text1=scr_omnitext(296);
        text2=scr_omnitext(297);
        text3=scr_omnitext(298)+string(global.besttime5)+"   "+scr_omnitext(299)+string(global.bestflags);
        }
    else if (selected==5){
        text1=scr_omnitext(300);
        text2=scr_omnitext(301);
        text3="";
        }
    else if (selected==6){
        text1=scr_omnitext(302);
        text2=scr_omnitext(303);
        text3=scr_omnitext(304);
        }
    else if (selected==7){
        text1=scr_omnitext(305)+best1;
        text2=scr_omnitext(306)+best2;
        if (global.annihilation)
            text3=scr_omnitext(307)+best3;
        else
            text3+="";
        }
    else if (selected==8){
        text1=scr_omnitext(577);
        text2=scr_omnitext(578);
        text3=scr_omnitext(579);
        }
    }