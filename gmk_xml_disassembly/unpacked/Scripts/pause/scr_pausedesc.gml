//MAIN
if (tier=="main"){
    if (selected==1){
        text1=scr_omnitext(124);
        if (global.reallyplay==1) text1=scr_omnitext(125);
        else if (global.difficulty==0) text1=scr_omnitext(126);
        else if (global.difficulty==1) text1=scr_omnitext(127);
        else if (global.difficulty==2) text1=scr_omnitext(128);
        else if (global.difficulty==3) text1=scr_omnitext(129);
        scr_timestring();
        text1+=timestring;
        text2=scr_omnitext(130);
        if (global.level<10) text2+=string(global.level);
        else text2+="x";
        text2+=scr_omnitext(131)+string(numrooms);
        if (global.computers>0)
            text3=scr_omnitext(270)+string(global.computers)+"/10";
        else
            text3="";
        }
    else if (selected==2){
        text1=scr_omnitext(133);
        text2=scr_omnitext(134);
        text3="";
        }
    else if (selected==3){
        text1=scr_omnitext(135);
        text2="";
        text3="";
        }
    else if (selected==4){
        text1=scr_omnitext(136);
        text2="";
        text3="";
        }
    else if (selected==5){
        text1=scr_omnitext(137);
        text2="";
        text3="";
        }
    else if (selected==6){
        if (global.reallyplay){
            text1=scr_omnitext(138);
            text2=scr_omnitext(139);
            text3="";
            }
        else{
            text1=scr_omnitext(140);
            text2=scr_omnitext(141);
            text3="";
            }
        }
    else if (selected==7){
        text1=scr_omnitext(142);
        text2=scr_omnitext(143);
        text3="";
        }
    else if (selected==8){
        text1=scr_omnitext(144);
        text2=scr_omnitext(145);
        text3="";
        }
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

//INFORMATION
else if (tier=="information"){
    if (selected==1){
        text1=scr_omnitext(146);
        text2="";
        text3="";
        }
    else if (selected==2){
        if (global.reallyplay){
            text1=scr_omnitext(161);
            text2=scr_omnitext(162);
            text3="";
            }
        else if (global.difficulty<2){
            text1=scr_omnitext(163);
            text2=scr_omnitext(164);
            text3=scr_omnitext(165);
            }
        else{
            text1=scr_omnitext(166);
            text2=scr_omnitext(167);
            text3=scr_omnitext(168);
            }
        }
    else if (selected==3){
        text1=scr_omnitext(169);
        text2=scr_omnitext(170);
        text3=scr_omnitext(171);
        }
    else if (selected==4){
        text1=scr_omnitext(172);
        text2=scr_omnitext(173);
        text3="";
        }
    else if (selected==5){
        if (global.blaster==0){
            text1=scr_omnitext(174);
            text2=scr_omnitext(175);
            text3=scr_omnitext(176);
            }
        else if (global.blaster==1){
            text1=scr_omnitext(177);
            text2=scr_omnitext(178);
            text3=scr_omnitext(179);
            }
        else if (global.blaster==2){
            text1=scr_omnitext(180);
            text2=scr_omnitext(181);
            text3=scr_omnitext(182);
            }
        else if (global.blaster==3){
            text1=scr_omnitext(183);
            text2=scr_omnitext(184);
            text3=scr_omnitext(185);
            }
        }
    else if (selected==6){
        if (global.blade==0){
            if (global.reallyplay){
                text1=scr_omnitext(186);
                text2=scr_omnitext(187);
                text3=scr_omnitext(188);
                }
            else{
                text1=scr_omnitext(186);
                text2=scr_omnitext(189);
                text3=scr_omnitext(190);
                }
            }
        else if (global.blade==1){
            text1=scr_omnitext(191);
            text2=scr_omnitext(192);
            text3=scr_omnitext(193);
            }
        else if (global.blade==2){
            text1=scr_omnitext(194);
            text2=scr_omnitext(195);
            text3=scr_omnitext(196);
            }
        else if (global.blade==3){
            text1=scr_omnitext(197);
            text2=scr_omnitext(198);
            text3=scr_omnitext(199);
            }
        }
    else if (selected==7){
        if (global.suit==0){
            if (global.reallyplay){
                text1=scr_omnitext(200);
                text2=scr_omnitext(201);
                text3=scr_omnitext(202);
                }
            else{
                text1=scr_omnitext(200);
                text2=scr_omnitext(203);
                text3=scr_omnitext(204);
                }
            }
        else if (global.suit==1){
            text1=scr_omnitext(205);
            text2=scr_omnitext(206);
            text3=scr_omnitext(207);
            }
        else if (global.suit==2){
            text1=scr_omnitext(208);
            text2=scr_omnitext(209);
            text3=scr_omnitext(210);
            }
        else if (global.suit==3){
            text1=scr_omnitext(211);
            text2=scr_omnitext(212);
            text3=scr_omnitext(213);
            }
        }
    else if (selected==8){
        if (global.computers>0){
            text1=scr_omnitext(214)+string(global.computers)+"/10";
            if (room==rom_quest3){
                text2=scr_omnitext(215);
                text3=scr_omnitext(216);
                }
            else{
                if (global.computers==10){
                    text2=scr_omnitext(217);
                    text3=scr_omnitext(218);
                    }
                else{
                    text2=scr_omnitext(219);
                    text3=scr_omnitext(220);
                    }
                }
            }
        else{
            if (global.reallyplay){
                text1=scr_omnitext(221);
                text2=scr_omnitext(222);
                text3="";
                }
            else{
                text1="???";
                text2="";
                text3="";
                }
            }
        }
    else if (selected==9){
        if (global.shipscan==1){
            text1=scr_omnitext(223);
            text2=scr_omnitext(224);
            text3="";
            }
        else{
            if (global.reallyplay){
                text1=scr_omnitext(225);
                text2=scr_omnitext(226);
                text3="";
                }
            else{
                text1="???";
                text2="";
                text3="";
                }
            }
        }
    else if (selected==10){
        if (global.patternscan){
            text1=scr_omnitext(227);
            text2=scr_omnitext(228);
            text3=scr_omnitext(229);
            }
        else{
            if (global.reallyplay){
                text1=scr_omnitext(230);
                text2="";
                text3="";
                }
            else{
                text1="???";
                text2="";
                text3="";
                }
            }
        }
    else if (selected==11){
        if (global.expel){
            text1=scr_omnitext(231);
            text2=scr_omnitext(232);
            text3=scr_omnitext(233);
            }
        else{
            if (global.reallyplay){
                text1=scr_omnitext(234);
                text2=scr_omnitext(235);
                text3="";
                }
            else{
                text1="???";
                text2="";
                text3="";
                }
            }
        }
    }

//SAVE FIRST
else if (tier=="savefirst"){
    if (selected==1){
        text1=scr_omnitext(236);
        text2="";
        text3="";
        }
    else if (selected==2){
        text1=scr_omnitext(237);
        text2="";
        text3="";
        }
    }
//MAP
else if (tier=="map"){
    if (global.retromode)
        text1=scr_omnitext(238);
    else
        text1=scr_omnitext(239);
    text2=scr_omnitext(240);
    text3=scr_omnitext(241);
    if (global.difficulty==2)
        text3=scr_omnitext(242);
    if (global.difficulty!=2 && !global.retromode)
        text2+=scr_omnitext(243);
    if (global.reallyplay){
        text2="";
        text3="";
        }
    if (global.bossmode){
        text2="";
        text3="";
        }
    if (global.forever){
        text2=scr_omnitext(244);
        text3="";
        }
    }
//WARP
else if (tier=="warp"){
    text1=scr_omnitext(245);
    text2=scr_omnitext(246);
    text3=scr_omnitext(247);
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
//BOSSMODE
else if (tier=="bossmode"){
    if (selected==1){
        if (global.forever){
            text1=scr_omnitext(256);
            scr_timestring();
            text1+=timestring;
            text2=scr_omnitext(130);
            if (global.level<10) text2+=string(global.level);
            else text2+="x";
            text3=scr_omnitext(257)+string(foreverlevel)+scr_omnitext(258)+string(obj_sabot.flags);
            }
        else{
            if (global.normalrush)
                text1=scr_omnitext(351)+" _"+scr_omnitext(319)+"\"+scr_omnitext(129);
            else
                text1=scr_omnitext(351)+" _"+scr_omnitext(320)+"\"+scr_omnitext(129);
            scr_timestring();
            text1+=timestring;
            text2=scr_omnitext(130);
            if (global.level<10) text2+=string(global.level);
            else text2+="x";
            text2+=scr_omnitext(260)+string(numrooms);
            }
        }
    else if (selected==2){
        if (global.forever)
            text1=scr_omnitext(261);
        else
            text1=scr_omnitext(262);
        text2="";
        text3="";
        }
    else if (selected==3){
        text1=scr_omnitext(263);
        text2=scr_omnitext(264);
        text3="";
        }
    else if (selected==4){
        text1=scr_omnitext(136);
        text2="";
        text3="";
        }
    else if (selected==5){
        text1=scr_omnitext(137);
        text2="";
        text3="";
        }
    else if (selected==6){
        text1=scr_omnitext(142);
        text2=scr_omnitext(143);
        text3="";
        }
    else if (selected==7){
        text1=scr_omnitext(144);
        text2=scr_omnitext(145);
        text3="";
        }
    }
