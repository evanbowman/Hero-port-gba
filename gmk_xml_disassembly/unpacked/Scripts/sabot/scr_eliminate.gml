//Boss mode Eliminator
if (global.bossmode && roomx==8 && roomy==9){
obj_sabot.explo[2]=1;
obj_sabot.shakescreen=6;
instance_create(x+160,y+160,obj_gigaexplo);
instance_create(x+160,y+160,obj_eliminator);
for (i=3;i<50;i+=1){
    (instance_create(x+160-i*3.2,y+160-i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    (instance_create(x+160+i*3.2,y+160-i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    (instance_create(x+160+i*3.2,y+160+i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    (instance_create(x+160-i*3.2,y+160+i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    }
}

else if (!global.bossmode && !global.forever){
//Check room
temprand=random(1);
if (temprand<0.02+0.005*global.level && global.eliminator=="!" && zonenum!=10){
if (global.hp>0 && global.level>=1 && obj_hero.chargewarp==0){
if (obj_hero.x>x+16 && obj_hero.x<x+304 && obj_hero.y>y+16 && obj_hero.y<y+304){
if (text1="" && text2="" && text3=""){
if (instance_number(obj_eyespy_s)==0 && instance_number(obj_annihilator_s)==0 && instance_number(obj_cage)==0 && instance_number(obj_wallshooter)==0 && instance_number(obj_generator)==0 && instance_number(obj_savepoint_s)==0 && instance_number(obj_door1)==0 && instance_number(obj_battledoor)==0 && instance_number(obj_itemspawner)==0 && instance_number(obj_secret)==0 && instance_number(obj_secret2)==0 && instance_number(obj_secret3)==0){
//Spawn
obj_flower.ready=0;
obj_hero.ignoreall=1;
obj_hero.alarm[0]=40;
obj_sabot.explo[2]=1;
with (obj_enemy)
    hp=0;
with (obj_flutter)
    instance_destroy();
obj_sabot.shakescreen=6;
instance_create(x+160,y+160,obj_gigaexplo);
instance_create(x+160,y+160,obj_eliminator);
for (i=3;i<50;i+=1){
    (instance_create(x+160-i*3.2,y+160-i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    (instance_create(x+160+i*3.2,y+160-i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    (instance_create(x+160+i*3.2,y+160+i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    (instance_create(x+160-i*3.2,y+160+i*3.2,obj_flame)).alarm[0]=10+random(50-i);
    }
}
}
}
}
}
}