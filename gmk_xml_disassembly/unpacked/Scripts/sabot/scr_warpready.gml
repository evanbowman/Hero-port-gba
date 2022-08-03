//Build list of savepoints
warplistlen=0;
warplistcur=0;
for (j=0;j<15;j+=1){
    for (i=0;i<15;i+=1){
        if (mitem[i,j]==1){
            mwarpx[warplistlen]=i;
            mwarpy[warplistlen]=j;
            warplistlen+=1;
            }
        }
    }
if (warplistlen==0){//No savepoints
    mwarpx[warplistlen]=roomx;
    mwarpy[warplistlen]=roomy;
    warplistlen=1;
    }
warpx=mwarpx[0];
warpy=mwarpy[0];
//Put warpx on last used savepoint
for (i=0;i<warplistlen;i+=1){
    if (mwarpx[i]==respawnroomx && mwarpy[i]==respawnroomy){
        warpx=mwarpx[i];
        warpy=mwarpy[i];
        warplistcur=i;
        }
    }
warplistlen-=1;

pause=1;
shakescreen=0;
with (obj_block2){
    x=xstart;
    y=ystart;
    }
instance_deactivate_all(1);
oldtext1=text1;
oldtext2=text2;
oldtext3=text3;
text1="";
text2="";
text3="";
tier="warp";
selected=1;
scr_pausetexts();
scr_pausedesc();
scr_sound("menuselect");
