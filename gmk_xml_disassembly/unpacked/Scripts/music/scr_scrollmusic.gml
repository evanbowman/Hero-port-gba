//Pick song at end of scroll

//Computer
if (instance_number(obj_computer)>0 && global.difficulty!=2)
    scr_wantsong(0);
//Boss
else if (boss && !global.reallyplay){
    if (instance_number(obj_tetron)>0 || instance_number(obj_warmachine)>0)
        scr_wantsong(14);
    else if (instance_number(obj_smasher)==1 && !global.bossmode)
        scr_wantsong(0);
    else
        scr_wantsong(13);
    }
//Zones
else{
    if (zonenum==1) scr_wantsong(1);
    else if (zonenum==2) scr_wantsong(2);
    else if (zonenum==3) scr_wantsong(3);
    else if (zonenum==4) scr_wantsong(4);
    else if (zonenum==5) scr_wantsong(5);
    else if (zonenum==6) scr_wantsong(6);
    else if (zonenum==7) scr_wantsong(7);
    else if (zonenum==8) scr_wantsong(8);
    else if (zonenum==9) scr_wantsong(9);
    else if (zonenum==10 || zonenum==22) scr_wantsong(10);
    else if (zonenum==11) scr_wantsong(10);
    else if (zonenum==12) scr_wantsong(10);
    else if (zonenum==13) scr_wantsong(10);
    else if (zonenum==14) scr_wantsong(9);
    else if (zonenum==15) scr_wantsong(9);
    else if (zonenum==16) scr_wantsong(9);
    else if (zonenum==17) scr_wantsong(9);
    else if (zonenum==18) scr_wantsong(7);
    else if (zonenum==19) scr_wantsong(9);
    else if (zonenum==20) scr_wantsong(13);
    else if (zonenum==21) scr_wantsong(15);
    else if (zonenum==24) scr_wantsong(13); //really
    else if (zonenum==99) scr_wantsong(0); //???
    else if (zonenum==23){ //forever
        if (foreverlevel==999)
            scr_wantsong(0);
        else{
            temp=foreverlevel mod 10;
            if (temp==0)
                temp=10;
            scr_wantsong(temp);
            }
        }
    }