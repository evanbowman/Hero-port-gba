scr_sound("pickup");
if (!(global.forever && (other.image_single==4 || other.image_single==10))){
    ignoreall=1;
    alarm[0]=80;
    }
obj_sabot.alarm[2]=0;
//Blaster
if (other.image_single==0){
    scr_levelup();
    with (obj_sabot){
        scr_wantsong(16);
        alarm[6]=80;
        }
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        global.blaster+=1;
        if (global.blaster==2){
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
    }
//Blade
if (other.image_single==1){
    scr_levelup();
    with (obj_sabot){
        scr_wantsong(16);
        alarm[6]=80;
        }
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        global.blade+=1;
        if (global.blade==1){
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
    }
//Suit
if (other.image_single==2){
    scr_levelup();
    with (obj_sabot){
        scr_wantsong(16);
        alarm[6]=80;
        }
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        global.suit+=1;
        if (global.suit==1){
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
    }
//Computer
if (other.image_single==3){
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        global.computers+=1;
        if (global.difficulty<2){
            if (global.computers==1){
                text1=scr_omnitext(397);
                text2=scr_omnitext(398);
                text3=scr_omnitext(399);
                }
            else if (global.computers==2){
                text1=scr_omnitext(400);
                text2=scr_omnitext(401);
                text3=scr_omnitext(402);
                }
            else if (global.computers==3){
                text1=scr_omnitext(403);
                text2=scr_omnitext(404);
                text3=scr_omnitext(405);
                }
            else if (global.computers==4){
                text1=scr_omnitext(406);
                text2=scr_omnitext(407);
                text3=scr_omnitext(408);
                }
            else if (global.computers==5){
                text1=scr_omnitext(409);
                text2=scr_omnitext(410);
                text3=scr_omnitext(411);
                }
            else if (global.computers==6){
                text1=scr_omnitext(412);
                text2=scr_omnitext(413);
                text3=scr_omnitext(414);
                }
            else if (global.computers==7){
                text1=scr_omnitext(415);
                text2=scr_omnitext(416);
                text3=scr_omnitext(417);
                }
            else if (global.computers==8){
                text1=scr_omnitext(418);
                text2=scr_omnitext(419);
                text3=scr_omnitext(420);
                }
            else if (global.computers==9){
                text1=scr_omnitext(421);
                text2=scr_omnitext(422);
                text3=scr_omnitext(423);
                }
            else if (global.computers==10){
                text1=scr_omnitext(424);
                text2=scr_omnitext(425);
                text3=scr_omnitext(426);
                }
            }
        else{
            if (global.computers==1){
                text1=scr_omnitext(427);
                text2=scr_omnitext(428);
                text3=scr_omnitext(429);
                }
            else if (global.computers==2){
                text1=scr_omnitext(430);
                text2=scr_omnitext(431);
                text3=scr_omnitext(432);
                }
            else if (global.computers==3){
                text1=scr_omnitext(433);
                text2=scr_omnitext(434);
                text3=scr_omnitext(435);
                }
            else if (global.computers==4){
                text1=scr_omnitext(436);
                text2=scr_omnitext(437);
                text3=scr_omnitext(438);
                }
            else if (global.computers==5){
                text1=scr_omnitext(439);
                text2=scr_omnitext(440);
                text3=scr_omnitext(441);
                }
            else if (global.computers==6){
                text1=scr_omnitext(442);
                text2=scr_omnitext(443);
                text3=scr_omnitext(444);
                }
            else if (global.computers==7){
                text1=scr_omnitext(445);
                text2=scr_omnitext(446);
                text3=scr_omnitext(447);
                }
            else if (global.computers==8){
                text1=scr_omnitext(448);
                text2=scr_omnitext(449);
                text3=scr_omnitext(450);
                }
            else if (global.computers==9){
                text1=scr_omnitext(451);
                text2=scr_omnitext(452);
                text3=scr_omnitext(453);
                }
            else if (global.computers==10){
                text1=scr_omnitext(454);
                text2=scr_omnitext(455);
                text3=scr_omnitext(456);
                }
            }
        }
    }
//Map
if (other.image_single==4 || other.image_single==10){
    global.shipscan=1;
    //Prepare hypergun
    if (other.image_single==10)
        obj_sabot.hypertemp=1;
    else
        obj_sabot.hypertemp=0;
    //Remove reallyjoel bosses
    if (room==rom_quest2){
        instance_activate_object(obj_itemspawner);
        with (obj_itemspawner){
            if (x>320 && x<640 && y>320 && y<640)
                instance_destroy();
        }
    }
    //Get map
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        if (global.forever){
            if (hypertemp){
                text1=scr_omnitext(584);
                global.hypergun=1;
                }
            else
                text1=scr_omnitext(386);
            text2=scr_omnitext(387);
            if (global.level<10){
                with (obj_hero)
                    scr_levelup();
                text3=scr_omnitext(388);
                }
            else
                text3="";
            }
        else{
            text1=scr_omnitext(389);
            if (global.difficulty==2)
                text2=scr_omnitext(390);
            else
                text2=scr_omnitext(224);
            text3="";
            }
        instance_activate_object(obj_itemspawner);
        mask_index=spr_mapoverlay;
        for (i=0;i<15;i+=1){
            for (j=0;j<15;j+=1){
                if (place_meeting(i*320,j*320,obj_itemspawner) && !place_meeting(i*320,j*320,obj_expel_s))
                    mitem[i,j]=2;
                }
            }
        mask_index=-1;
        instance_deactivate_object(obj_itemspawner);
        }
    }
//Pattern scanner
if (other.image_single==5){
    scr_levelup();
    with (obj_sabot){
        scr_wantsong(16);
        alarm[6]=80;
        }
    global.patternscan=1;
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        text1=scr_omnitext(227);
        text2=scr_omnitext(228);
        text3=scr_omnitext(229);
        instance_activate_object(obj_savepoint_s);
        mask_index=spr_mapoverlay;
        for (i=0;i<15;i+=1){
            for (j=0;j<15;j+=1){
                if (place_meeting(i*320,j*320,obj_savepoint_s))
                    mitem[i,j]=1;
                }
            }
        mask_index=-1;
        instance_deactivate_object(obj_savepoint_s);
        }
    }
//Expel
if (other.image_single==7){
    with (obj_sabot){
        global.expel=1;
        mitemclear[roomx,roomy]=1;
        text1=scr_omnitext(391);
        text2=scr_omnitext(392);
        text3=scr_omnitext(393);
        }
    }
//Level up
if (other.image_single==8){
    scr_levelup();
    with (obj_sabot){
        scr_wantsong(16);
        alarm[6]=80;
        }
    with (obj_sabot){
        global.eliminator="X";
        text1=scr_omnitext(394);
        text2=scr_omnitext(395);
        text3="";
        }
    }
//Extraspecial
if (other.image_single==9){
    global.extraspecial="X";
    with (obj_sabot){
        mitemclear[roomx,roomy]=1;
        text1=scr_omnitext(396);
        text2=scr_omnitext(515);
        text3=scr_omnitext(516);
        }
    }
with (other)
    instance_destroy();
cannotwarp=0;
