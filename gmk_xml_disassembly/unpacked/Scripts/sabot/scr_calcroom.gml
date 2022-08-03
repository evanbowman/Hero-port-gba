//argument0 1 is only the save points
//argument0 2 is only the bosses/items/computers

if (argument0==0){
//CALCULATE ROOM EXITS
//find connections
r=0; u=0; l=0; d=0;
for (i=0;i<18;i+=1){
    if (place_free(x+304,y+16+i*16))
        r=1;
    }
for (i=0;i<18;i+=1){
    if (place_free(x+16+i*16,y))
        u=1;
    }
for (i=0;i<18;i+=1){
    if (place_free(x,y+16+i*16))
        l=1;
    }
for (i=0;i<18;i+=1){
    if (place_free(x+16+i*16,y+304))
        d=1;
    }
if (r && !u && !l && !d)
    mroom[roomx,roomy]=1;
else if  (!r && u && !l && !d)
    mroom[roomx,roomy]=2;
else if  (!r && !u && l && !d)
    mroom[roomx,roomy]=3;
else if  (!r && !u && !l && d)
    mroom[roomx,roomy]=4;
else if  (r && u && !l && !d)
    mroom[roomx,roomy]=5;
else if  (!r && u && l && !d)
    mroom[roomx,roomy]=6;
else if  (!r && !u && l && d)
    mroom[roomx,roomy]=7;
else if  (r && !u && !l && d)
    mroom[roomx,roomy]=8;
else if  (r && !u && l && !d)
    mroom[roomx,roomy]=9;
else if  (!r && u && !l && d)
    mroom[roomx,roomy]=10;
else if  (r && u && !l && d)
    mroom[roomx,roomy]=11;
else if  (r && u && l && !d)
    mroom[roomx,roomy]=12;
else if  (!r && u && l && d)
    mroom[roomx,roomy]=13;
else if  (r && !u && l && d)
    mroom[roomx,roomy]=14;
else if  (r && u && l && d)
    mroom[roomx,roomy]=15;
else if  (!r && !u && !l && !d)
    mroom[roomx,roomy]=16;
}

//ITEM ARRAY (SAVES)
if (argument0==0 || argument0==1){
    if (instance_number(obj_savepoint_s)>0)
        mitem[roomx,roomy]=1;
    }
//ITEM ARRAY (ITEMS)
if (argument0==0 || argument0==2){
    if (instance_number(obj_itemspawner)>0)
        mitem[roomx,roomy]=2;
    }
