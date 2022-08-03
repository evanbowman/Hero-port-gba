//Init once
if (!foreverinit){
    foreverinit=1;
    foreverlevel=1;
    levelzero=0;
    masterxsize=3;
    masterysize=2;
    flags=0;
    scr_foreverstats();
    }
//Init
for (i=0;i<15;i+=1){
    for (j=0;j<15;j+=1)
        clearedroom[i,j]=0;
    }
if (foreverlevel mod 2==0)
    masterxsize+=1;
else
    masterysize+=1;
if (masterxsize>5)
    masterxsize=5;
if (masterysize>5)
    masterysize=5;
if (foreverlevel==-1){
    levelzero=1;
    foreverlevel=999;
    }
if (foreverlevel==999){
    masterxsize=3;
    masterysize=3;
    }
terrorx=floor(random(masterxsize));
terrory=floor(random(masterysize));
flood=random(1);
if (flood>0.75)
    flood=1;
else
    flood=0;
floodheight=floor(random(2)+1);
roomx=floor(random(masterxsize));
roomy=floor(random(masterysize));
respawnroomx=roomx;
respawnroomy=roomy;
x=roomx*320;
y=roomy*320;
goalroomx=floor(random(masterxsize));
goalroomy=floor(random(masterysize));
while (goalroomx==roomx && goalroomy==roomy){
    goalroomx=floor(random(masterxsize));
    goalroomy=floor(random(masterysize));
    }
scanroomx=floor(random(masterxsize));
scanroomy=floor(random(masterysize));
while ((scanroomx==respawnroomx && scanroomy==respawnroomy) || (scanroomx==goalroomx && scanroomy==goalroomy)){
    scanroomx=floor(random(masterxsize));
    scanroomy=floor(random(masterxsize));
    }
roomx=floor(random(masterxsize));
roomy=floor(random(masterysize));
if (foreverlevel==0){
    scanroomx=15;
    scanroomy=15;
    goalroomx=15;
    goalroomy=15;
    terrorx=15;
    terrory=15;
    }

tempid=instance_create(x,y,obj_loc);
tempid.zonenum=23;
tempid.threat=0;
if (tempid.threat>10)
    tempid.threat=10;
tempid.text=1;
tempid.text1=509;
tempid.text2=0;
tempid.text3=0;
if (foreverlevel==1){
    tempid.text1=510;
    tempid.text2=511;
    tempid.text3=0;
    }

//Master room sweep
for (froomx=0;froomx<masterxsize;froomx+=1){
    for (froomy=0;froomy<masterysize;froomy+=1){
        //Generate mesh
        for (m=0;m<10;m+=1){
            for (n=0;n<10;n+=1){
                mesh[m,n]=1;
                }
            }
        //Middle hole
        if (foreverlevel==1 && froomx==respawnroomx && froomy==respawnroomy){
            for (m=3;m<7;m+=1){
                for (n=3;n<8;n+=1){
                    mesh[m,n]=0;
                    }
                }
            }
        else{
            for (m=4;m<6;m+=1){
                for (n=4;n<6;n+=1){
                    mesh[m,n]=0;
                    }
                }
            }
        //Generate holes
        numenemies=0;
        for (a=0;a<7;a+=1){
            xdefine=1+floor(random(8));
            ydefine=1+floor(random(8));
            xsize=1+floor(random(2));
            ysize=1+floor(random(2));
            if (xdefine+xsize>8)
                xdefine-=xsize;
            if (ydefine+ysize>8)
                ydefine-=ysize;
            xsize+=xdefine;
            ysize+=ydefine;
            for (b=xdefine;b<xsize;b+=1){
                for (c=ydefine;c<ysize;c+=1){
                    mesh[b,c]=0;
                    }
                }
            if (xdefine+1>5){
                tunneldist=2+floor(random(3));
                for (d=xdefine+1;d>tunneldist;d-=1)
                    mesh[d,ydefine+1]=0;
                }
            else if (xdefine+1<=5){
                tunneldist=5+floor(random(3));
                for (d=xdefine+1;d<tunneldist;d+=1)
                    mesh[d,ydefine+1]=0;
                }
            if (ydefine+1>5){
                tunneldist=2+floor(random(3));
                for (d=ydefine+1;d>tunneldist;d-=1)
                    mesh[xdefine+1,d]=0;
                }
            else if (ydefine+1<=5){
                tunneldist=5+floor(random(3));
                for (d=ydefine+1;d<tunneldist;d+=1)
                    mesh[xdefine+1,d]=0;
                }
            //Place enemies inside define
            if (foreverlevel<999 && xdefine>2 && xdefine<7 && ydefine>2 && ydefine<7 && !(froomx==respawnroomx && froomy==respawnroomy) && !(froomx==goalroomx && froomy==goalroomy) && !(froomx==scanroomx && froomy==scanroomy) && !(froomx==terrorx && froomy==terrory)){
                enemytemp=foreverlevel;
                if (enemytemp>10)
                    enemytemp=10;
                temp=floor(random(5)+enemytemp);
                if (numenemies>=4)
                    temp=0;
                xfactor=floor(random(16))*2;
                yfactor=floor(random(16))*2;
                if (!place_meeting(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_spawner)){
                    if (temp==1) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_drone_s);
                    else if (temp==2) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_reaver_s);
                    else if (temp==3) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_crusher_s);
                    else if (temp==4) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_spew_s);
                    else if (temp==5) (instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_boltaray_s)).mask_index=spr_hunter;
                    else if (temp==6) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_chainsnake_s);
                    else if (temp==7) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_soldier_s);
                    else if (temp==8) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_phaze_s);
                    else if (temp==9) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_hunter_s);
                    else if (temp==10) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_direviper_s);
                    else if (temp==11) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_mode_s);
                    else if (temp==12) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_mother_s);
                    else if (temp==13) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_eidolon_s);
                    else if (temp>=14) instance_create(froomx*320+xdefine*32+2+xfactor,froomy*320+ydefine*32+2+yfactor,obj_annihilator_s);
                    numenemies+=1;
                    }
                }
            }
        //place chests/flags in mesh
        if (foreverlevel!=999 && !(froomx==respawnroomx && froomy==respawnroomy) && !(froomx==goalroomx && froomy==goalroomy) && !(froomx==scanroomx && froomy==scanroomy)){
            for (m=0;m<10;m+=1){
                for (n=0;n<10;n+=1){
                    if (m>1 && m<9 && n>1 && n<9 && mesh[m,n]==0){
                        temprand=random(1);
                        if (temprand<0.05)
                            mesh[m,n]=2;
                        }
                    }
                }
            }
        //Render mesh
        for (m=0;m<10;m+=1){
            for (n=0;n<10;n+=1){
                if (mesh[m,n]==2){
                    //Bomb/flag/chest
                    flagtemp=random(1);
                    if (flagtemp<0.0667)
                        tempid=instance_create(froomx*320+m*32+16,froomy*320+n*32+16,obj_foreverbomb);
                    else if (flagtemp<0.7)
                        tempid=instance_create(froomx*320+m*32+16,froomy*320+n*32+16,obj_flag);
                    else
                        tempid=instance_create(froomx*320+m*32+16,froomy*320+n*32+16,obj_chest);
                    }
                //blocks
                if (mesh[m,n]==1){
                    typetemp=random(1);
                    tempid=instance_create(froomx*320+m*32,froomy*320+n*32,obj_blockmed);
                    //foreverblocks
                    temprand=random(1);
                    if (foreverlevel!=999 && temprand<0.07 && !(flood && tempid.y>masterysize*320-floodheight*48)){
                        tempid.sprite_index=spr_foreverblock;
                        tempid.image_single=foreverlevel;
                        tempid.depth-=1;
                        //Hiddengoal block
                        if (random(1)<0.1 && !(m==0 || m==9 || n==0 || n==9) && !(froomx==goalroomx && froomy==goalroomy) && !(froomx==scanroomx && froomy==scanroomy) && !(froomx==respawnroomx && froomy==respawnroomy) && !(froomx==terrorx && froomy==terrory)){
                            tempid.image_single+=1;
                            tempid.hiddengoal=1;
                            }
                        }
                    //not bombable
                    if (m==0 || m==9 || n==0 || n==9)
                        tempid.nobomb=1;
                    //terror
                    terrorfacing=floor(random(2));
                    if (m>0 && m<9){
                        if (foreverlevel<999 && foreverlevel>1 && froomx==terrorx && froomy==terrory && !placedterror && random(1)<0.3 && ((terrorfacing==0 && mesh[m-1,n]==0) || (terrorfacing==1 && mesh[m+1,n]==0)) && !(froomx==goalroomx && froomy==goalroomy) && !(froomx==scanroomx && froomy==scanroomy) && !(froomx==respawnroomx && froomy==respawnroomy)){
                            //placedterror=1;
                            (instance_create(tempid.x,tempid.y,obj_terror_s)).facing=terrorfacing;
                            tempid.nobomb=1;
                            tempid.nomove=1;
                            }
                        }
                    //crumbled world
                    if ((foreverlevel mod 10==1 || foreverlevel mod 10==4 || foreverlevel mod 10==7) && m>0 && m<8 && n>0 && n<8){
                        if (!tempid.nomove){
                            tempid.x+=floor(random(2))*16;
                            tempid.y+=floor(random(2))*16;
                            }
                        }
                    //pipe world
                    else if ((foreverlevel mod 10==2 || foreverlevel mod 10==5 || foreverlevel mod 10==8) && m>0 && m<9 && n>0 && n<9){
                        temprand=random(1);
                        if (temprand<0.03){
                            instance_create(tempid.x,tempid.y,obj_pipe2_s);
                            instance_create(tempid.x+16,tempid.y,obj_pipe2_s);
                            if (m<8){
                                if (place_free(tempid.x+32,tempid.y))
                                    instance_create(tempid.x+32,tempid.y,obj_pipe2_s);
                                if (place_free(tempid.x+48,tempid.y))
                                    instance_create(tempid.x+48,tempid.y,obj_pipe2_s);
                                }
                            if (m<7){
                                if (place_free(tempid.x+64,tempid.y))
                                    instance_create(tempid.x+64,tempid.y,obj_pipe2_s);
                                if (place_free(tempid.x+80,tempid.y))
                                    instance_create(tempid.x+80,tempid.y,obj_pipe2_s);
                                }
                            with (tempid)
                                instance_destroy();
                            }
                        else if (temprand<0.06){
                            instance_create(tempid.x,tempid.y+16,obj_pipe2_s);
                            instance_create(tempid.x+16,tempid.y+16,obj_pipe2_s);
                            if (m<8){
                                if (place_free(tempid.x+32,tempid.y+16))
                                    instance_create(tempid.x+32,tempid.y+16,obj_pipe2_s);
                                if (place_free(tempid.x+48,tempid.y+16))
                                    instance_create(tempid.x+48,tempid.y+16,obj_pipe2_s);
                                }
                            if (m<7){
                                if (place_free(tempid.x+64,tempid.y+16))
                                    instance_create(tempid.x+64,tempid.y+16,obj_pipe2_s);
                                if (place_free(tempid.x+80,tempid.y+16))
                                    instance_create(tempid.x+80,tempid.y+16,obj_pipe2_s);
                                }
                            with (tempid)
                                instance_destroy();
                            }
                        else if (temprand<0.09){
                            instance_create(tempid.x,tempid.y,obj_pipe1_s);
                            instance_create(tempid.x,tempid.y+16,obj_pipe1_s);
                            if (n<8){
                                if (place_free(tempid.x,tempid.y+32))
                                    instance_create(tempid.x,tempid.y+32,obj_pipe1_s);
                                if (place_free(tempid.x,tempid.y+48))
                                    instance_create(tempid.x,tempid.y+48,obj_pipe1_s);
                                }
                            if (n<7){
                                if (place_free(tempid.x,tempid.y+64))
                                    instance_create(tempid.x,tempid.y+64,obj_pipe1_s);
                                if (place_free(tempid.x,tempid.y+80))
                                    instance_create(tempid.x,tempid.y+80,obj_pipe1_s);
                                }
                            with (tempid)
                                instance_destroy();
                            }
                        else if (temprand<0.12){
                            instance_create(tempid.x+16,tempid.y,obj_pipe1_s);
                            instance_create(tempid.x+16,tempid.y+16,obj_pipe1_s);
                            if (n<8){
                                if (place_free(tempid.x+16,tempid.y+32))
                                    instance_create(tempid.x+16,tempid.y+32,obj_pipe1_s);
                                if (place_free(tempid.x+16,tempid.y+48))
                                    instance_create(tempid.x+16,tempid.y+48,obj_pipe1_s);
                                }
                            if (n<7){
                                if (place_free(tempid.x+16,tempid.y+64))
                                    instance_create(tempid.x+16,tempid.y+64,obj_pipe1_s);
                                if (place_free(tempid.x+16,tempid.y+80))
                                    instance_create(tempid.x+16,tempid.y+80,obj_pipe1_s);
                                }
                            with (tempid)
                                instance_destroy();
                            }
                        }
                    //sand world
                    else if ((foreverlevel mod 10==3 || foreverlevel mod 10==6 || foreverlevel mod 10==9) && m>0 && m<9 && n>0 && n<9 && foreverlevel!=999){
                        temprand=random(1);
                        if (temprand<0.4 && !(flood && tempid.y>masterysize*320-floodheight*48)){
                            instance_create(tempid.x,tempid.y,obj_dirtbig_s);
                            with (tempid)
                                instance_destroy();
                            }
                        }
                    //death world
                    else if ((foreverlevel mod 10==0) && m>1 && m<8 && n>1 && n<8 && !(froomx==scanroomx && froomy==scanroomy)){
                        temprand=random(1);
                        if (temprand<0.4){
                            instance_create(tempid.x+8,tempid.y+8,obj_bolt_s);
                            instance_create(tempid.x+24,tempid.y+8,obj_bolt_s);
                            instance_create(tempid.x+8,tempid.y+24,obj_bolt_s);
                            instance_create(tempid.x+24,tempid.y+24,obj_bolt_s);
                            with (tempid)
                                instance_destroy();
                            }
                        }
                    }
                }
            }
        // -Master room sweep
        }
    }
//Cut paths between rooms
for (froomx=0;froomx<masterxsize;froomx+=1){
    for (froomy=0;froomy<masterysize;froomy+=1){
        //cutting dir
        temp=floor(random(4));
        //overrides
        if (froomx==1 || froomx==masterxsize-1)
            temp=2+floor(random(2));
        if (froomy==1 || froomy==masterxsize-1)
            temp=1+floor(random(2))*2;
        if (froomx==masterxsize-1 && froomy==masterysize-1)
            temp=3;
        //destroy cuts
        temp5=floor(random(3));
        if (temp5==0)
            temp=0;
        //hor
        if ((temp==1 || temp==3 || froomx==respawnroomx || froomx==goalroomx || froomx==scanroomx) && froomy>0){
            temp2=floor(random(4)+3);//loc
            temp3=1+floor(random(3));//size
            if (temp2+temp3>9)
                temp2-=temp3;
            for (t=0;t<temp3;t+=1){
                instance_create(froomx*320+temp2*32+t*32,froomy*320-160,obj_blocktall3)
                instance_create(froomx*320+temp2*32+t*32,froomy*320,obj_blocktall3)
                instance_create(froomx*320+temp2*32+t*32+16,froomy*320-160,obj_blocktall3)
                instance_create(froomx*320+temp2*32+t*32+16,froomy*320,obj_blocktall3)
                }
            }
        //ver
        if ((temp==2 || temp==3 || froomy==respawnroomy || froomy==goalroomy || froomy==scanroomy) && froomx>0){
            temp2=floor(random(4)+3);//loc
            temp3=1+floor(random(3));//size
            if (temp2+temp3>9)
                temp2-=temp3;
            for (t=0;t<temp3;t+=1){
                instance_create(froomx*320-160,froomy*320+temp2*32+t*32,obj_blockwide3)
                instance_create(froomx*320,froomy*320+temp2*32+t*32,obj_blockwide3)
                instance_create(froomx*320-160,froomy*320+temp2*32+t*32+16,obj_blockwide3)
                instance_create(froomx*320,froomy*320+temp2*32+t*32+16,obj_blockwide3)
                }
            }
        }
    }
with (obj_blockmed){
    if (place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner))
        instance_destroy();
    }
with (obj_chest){
    if (!place_free(x,y) || place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner))
        instance_destroy();
    }
with (obj_pipe1_s){
    if (!place_free(x,y) || place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner))
        instance_destroy();
    }
with (obj_pipe2_s){
    if (!place_free(x,y) || place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner))
        instance_destroy();
    }
with (obj_flag){
    if (!place_free(x,y) || place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_dirtbig_s) || place_meeting(x,y,obj_pipe1_s) || place_meeting(x,y,obj_pipe2_s))
        instance_destroy();
    }
with (obj_dirtbig_s){
    mask_index=spr_blockbig;
    if (place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner))
        instance_destroy();
    mask_index=spr_basebig2;
    }
with (obj_bolt_s){
    if (!place_free(x,y) || place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner))
        instance_destroy();
    }
with (obj_terror_s){
    if (place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_spawner) || place_meeting(x,y,obj_dirtbig_s) || place_meeting(x,y,obj_pipe1_s) || place_meeting(x,y,obj_pipe2_s) || place_free(x,y))
        instance_destroy();
    }
with (obj_foreverbomb){
    if (!place_free(x,y) || place_meeting(x,y,obj_blockx) || place_meeting(x,y,obj_dirtbig_s) || place_meeting(x,y,obj_pipe1_s) || place_meeting(x,y,obj_pipe2_s))
        instance_destroy();
    }
with (obj_blockx)
        instance_destroy();
//Remove pipes that shouldn't be there
with (obj_pipe1_s){
    if ((x mod 320)>=128 && (x mod 320)<=192 && (y mod 320)>=128 && (y mod 320)<=192)
        instance_destroy();
    }
with (obj_pipe2_s){
    if ((x mod 320)>=128 && (x mod 320)<=192 && (y mod 320)>=128 && (y mod 320)<=192)
        instance_destroy();
    }
//Place ship
if (foreverlevel==1){
    instance_create(respawnroomx*320+128,respawnroomy*320+208,obj_blockmed);
    instance_create(respawnroomx*320+160,respawnroomy*320+208,obj_blockmed);
    instance_create(respawnroomx*320+128,respawnroomy*320+176,obj_ship);
    }
//Place goal
if (foreverlevel<999){
    instance_create(goalroomx*320+160,goalroomy*320+160,obj_forevergoal);
    instance_create(goalroomx*320+128,goalroomy*320+128,obj_block);
    instance_create(goalroomx*320+128,goalroomy*320+176,obj_block);
    instance_create(goalroomx*320+176,goalroomy*320+128,obj_block);
    instance_create(goalroomx*320+176,goalroomy*320+176,obj_block);
    }
//Place scan
if (foreverlevel<999){
    instance_create(scanroomx*320+160,scanroomy*320+160,obj_shipscan_s);
    }
//Flood the bottom
if (foreverlevel!=999 && flood){
    for (a=0;a<masterxsize;a+=1){
        for (i=0;i<7;i+=1){
            for (j=0;j<floodheight;j+=1){
                instance_create(a*320+i*48,(masterysize*320)-j*48,obj_liquid2big_s);
                }
            }
        for (i=0;i<5;i+=1)
            instance_create(a*320+i*64,(masterysize*320)-(floodheight-1)*48-16,obj_liquid1long_s);
        }
    }
//Don't allow overlapping foreverblocks
with (obj_block){
    if (sprite_index==spr_foreverblock){
        if (place_meeting(x,y,obj_block)){
            sprite_index=spr_blockmed;
            image_single=0;
            hiddengoal=0;
            }
        }
    }
if (levelzero)
    foreverlevel=-1;